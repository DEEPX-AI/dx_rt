//
// Copyright (C) 2018- DEEPX Ltd.
// All rights reserved.
//
// This software is the property of DEEPX and is provided exclusively to customers
// who are supplied with DEEPX NPU (Neural Processing Unit).
// Unauthorized sharing or usage is strictly prohibited by law.
//

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using DxEngine;
using OpenCvSharp;

namespace DxEngine.Examples.ObjectDetection
{
    /// <summary>
    /// YOLOv5 object detection with synchronous inference.
    /// C# port of yolov5_sync.py
    /// </summary>
    public class YoloV5
    {
        private readonly string _modelPath;
        private readonly InferenceEngineNative _engine;
        private readonly int _inputHeight;
        private readonly int _inputWidth;

        // Detection thresholds
        private readonly float _objThreshold = 0.25f;
        private readonly float _scoreThreshold = 0.3f;
        private readonly float _nmsThreshold = 0.45f;

        // COCO 80 class labels
        private readonly string[] _classes;
        private readonly Scalar[] _colorPalette;

        // Per-image state
        private int _imgWidth;
        private int _imgHeight;
        private float _gain;
        private (int top, int left) _pad;

        public YoloV5(string modelPath)
        {
            _modelPath = modelPath;

            var option = new InferenceOption
            {
                UseOrt = true,  // YOLOv5 requires ONNX Runtime for CPU pre/post processing
                BufferCount = 6
            };

            _engine = new InferenceEngineNative(modelPath, option);

            // Get input tensor shape
            var inputInfo = _engine.InputTensorInfo;
            if (inputInfo.Count > 0)
            {
                var shape = inputInfo[0].Shape;
                // Assume NHWC format: [batch, height, width, channels]
                _inputHeight = shape.Length > 1 ? shape[1] : 640;
                _inputWidth = shape.Length > 2 ? shape[2] : 640;
            }
            else
            {
                _inputHeight = 640;
                _inputWidth = 640;
            }

            Console.WriteLine($"\n[INFO] Model loaded: {modelPath}");
            Console.WriteLine($"[INFO] Model input size (WxH): {_inputWidth}x{_inputHeight}");

            // Initialize COCO labels
            _classes = GetCocoLabels();
            
            // Generate random color palette
            var random = new Random(42);
            _colorPalette = new Scalar[_classes.Length];
            for (int i = 0; i < _classes.Length; i++)
            {
                _colorPalette[i] = new Scalar(random.Next(256), random.Next(256), random.Next(256));
            }
        }

        /// <summary>
        /// Letterbox resize with padding.
        /// </summary>
        private Mat Letterbox(Mat img, Size newShape)
        {
            var shape = new Size(img.Cols, img.Rows);
            float r = Math.Min((float)newShape.Height / shape.Height, (float)newShape.Width / shape.Width);

            var newUnpad = new Size((int)Math.Round(shape.Width * r), (int)Math.Round(shape.Height * r));
            float dw = (newShape.Width - newUnpad.Width) / 2f;
            float dh = (newShape.Height - newUnpad.Height) / 2f;

            Mat resized = img;
            if (shape.Width != newUnpad.Width || shape.Height != newUnpad.Height)
            {
                resized = new Mat();
                Cv2.Resize(img, resized, newUnpad, interpolation: InterpolationFlags.Linear);
            }

            int top = (int)Math.Round(dh - 0.1);
            int bottom = (int)Math.Round(dh + 0.1);
            int left = (int)Math.Round(dw - 0.1);
            int right = (int)Math.Round(dw + 0.1);

            var padded = new Mat();
            Cv2.CopyMakeBorder(resized, padded, top, bottom, left, right, BorderTypes.Constant, new Scalar(114, 114, 114));

            _pad = (top, left);

            if (resized != img)
                resized.Dispose();

            return padded;
        }

        /// <summary>
        /// Convert detection coordinates back to original image coordinates.
        /// </summary>
        private List<Detection> ConvertToOriginalCoordinates(List<Detection> detections)
        {
            foreach (var det in detections)
            {
                det.X1 = Math.Clamp((det.X1 - _pad.left) / _gain, 0, _imgWidth - 1);
                det.Y1 = Math.Clamp((det.Y1 - _pad.top) / _gain, 0, _imgHeight - 1);
                det.X2 = Math.Clamp((det.X2 - _pad.left) / _gain, 0, _imgWidth - 1);
                det.Y2 = Math.Clamp((det.Y2 - _pad.top) / _gain, 0, _imgHeight - 1);
            }
            return detections;
        }

        /// <summary>
        /// Draw detection boxes on the image.
        /// </summary>
        private void DrawDetections(Mat img, List<Detection> detections)
        {
            foreach (var det in detections)
            {
                var color = _colorPalette[det.ClassId];
                var pt1 = new Point((int)det.X1, (int)det.Y1);
                var pt2 = new Point((int)det.X2, (int)det.Y2);

                Cv2.Rectangle(img, pt1, pt2, color, 2);

                string label = $"{_classes[det.ClassId]}: {det.Score:F2}";
                var textSize = Cv2.GetTextSize(label, HersheyFonts.HersheySimplex, 0.5, 1, out int baseline);

                int labelX = (int)det.X1;
                int labelY = (int)det.Y1 - 10 > textSize.Height ? (int)det.Y1 - 10 : (int)det.Y1 + 10;

                Cv2.Rectangle(img, 
                    new Point(labelX, labelY - textSize.Height),
                    new Point(labelX + textSize.Width, labelY + textSize.Height),
                    color, -1);

                Cv2.PutText(img, label, new Point(labelX, labelY),
                    HersheyFonts.HersheySimplex, 0.5, new Scalar(0, 0, 0), 1, LineTypes.AntiAlias);
            }
        }

        /// <summary>
        /// Preprocess image for inference.
        /// </summary>
        private Mat Preprocess(Mat img)
        {
            _imgHeight = img.Rows;
            _imgWidth = img.Cols;
            _gain = Math.Min((float)_inputHeight / _imgHeight, (float)_inputWidth / _imgWidth);

            // Convert BGR to RGB
            var rgb = new Mat();
            Cv2.CvtColor(img, rgb, ColorConversionCodes.BGR2RGB);

            // Apply letterbox
            var letterboxed = Letterbox(rgb, new Size(_inputWidth, _inputHeight));
            rgb.Dispose();

            return letterboxed;
        }

        /// <summary>
        /// Postprocess inference output to get detections.
        /// </summary>
        private List<Detection> Postprocess(byte[] outputData)
        {
            var detections = new List<Detection>();

            // Convert byte array to float array
            int floatCount = outputData.Length / sizeof(float);
            var outputs = new float[floatCount];
            Buffer.BlockCopy(outputData, 0, outputs, 0, outputData.Length);

            // YOLOv5 output shape: [1, num_boxes, 85] for COCO (4 box + 1 obj + 80 classes)
            int numClasses = _classes.Length;
            int numAttribs = 5 + numClasses; // x, y, w, h, obj_conf, class_scores...
            int numBoxes = floatCount / numAttribs;

            var boxes = new List<Rect>();
            var confidences = new List<float>();
            var classIds = new List<int>();

            for (int i = 0; i < numBoxes; i++)
            {
                int offset = i * numAttribs;
                float objConf = outputs[offset + 4];

                if (objConf < _objThreshold)
                    continue;

                // Find best class
                float maxClassScore = 0;
                int maxClassId = 0;
                for (int c = 0; c < numClasses; c++)
                {
                    float classScore = outputs[offset + 5 + c];
                    if (classScore > maxClassScore)
                    {
                        maxClassScore = classScore;
                        maxClassId = c;
                    }
                }

                float confidence = objConf * maxClassScore;
                if (confidence < _scoreThreshold)
                    continue;

                // Center x, y, width, height
                float cx = outputs[offset + 0];
                float cy = outputs[offset + 1];
                float w = outputs[offset + 2];
                float h = outputs[offset + 3];

                // Convert to x1, y1, w, h for NMS
                int x1 = (int)(cx - w / 2);
                int y1 = (int)(cy - h / 2);

                boxes.Add(new Rect(x1, y1, (int)w, (int)h));
                confidences.Add(confidence);
                classIds.Add(maxClassId);
            }

            if (boxes.Count == 0)
                return detections;

            // Apply NMS (custom implementation)
            var indices = ApplyNMS(boxes, confidences, _nmsThreshold);

            foreach (int idx in indices)
            {
                var box = boxes[idx];
                detections.Add(new Detection
                {
                    X1 = box.X,
                    Y1 = box.Y,
                    X2 = box.X + box.Width,
                    Y2 = box.Y + box.Height,
                    Score = confidences[idx],
                    ClassId = classIds[idx]
                });
            }

            return detections;
        }

        /// <summary>
        /// Apply Non-Maximum Suppression.
        /// </summary>
        private List<int> ApplyNMS(List<Rect> boxes, List<float> confidences, float nmsThreshold)
        {
            var indices = new List<int>();
            var sortedIndices = Enumerable.Range(0, confidences.Count)
                .OrderByDescending(i => confidences[i])
                .ToList();

            var suppressed = new bool[boxes.Count];

            foreach (int i in sortedIndices)
            {
                if (suppressed[i])
                    continue;

                indices.Add(i);

                for (int j = 0; j < boxes.Count; j++)
                {
                    if (suppressed[j] || i == j)
                        continue;

                    float iou = ComputeIoU(boxes[i], boxes[j]);
                    if (iou > nmsThreshold)
                        suppressed[j] = true;
                }
            }

            return indices;
        }

        /// <summary>
        /// Compute Intersection over Union (IoU) of two boxes.
        /// </summary>
        private float ComputeIoU(Rect a, Rect b)
        {
            int x1 = Math.Max(a.X, b.X);
            int y1 = Math.Max(a.Y, b.Y);
            int x2 = Math.Min(a.X + a.Width, b.X + b.Width);
            int y2 = Math.Min(a.Y + a.Height, b.Y + b.Height);

            int intersectionArea = Math.Max(0, x2 - x1) * Math.Max(0, y2 - y1);
            int areaA = a.Width * a.Height;
            int areaB = b.Width * b.Height;
            int unionArea = areaA + areaB - intersectionArea;

            return unionArea > 0 ? (float)intersectionArea / unionArea : 0;
        }

        /// <summary>
        /// Run inference on a single image.
        /// </summary>
        public void ImageInference(string imagePath, bool display = true)
        {
            var swTotal = Stopwatch.StartNew();

            var img = Cv2.ImRead(imagePath);
            if (img.Empty())
            {
                Console.WriteLine($"[ERROR] Failed to load image: {imagePath}");
                return;
            }

            Console.WriteLine($"\n[INFO] Input image: {imagePath}");
            Console.WriteLine($"[INFO] Image resolution (WxH): {img.Cols}x{img.Rows}");

            var swPreprocess = Stopwatch.StartNew();
            var inputTensor = Preprocess(img);
            swPreprocess.Stop();

            // Convert to byte array
            var inputData = new byte[inputTensor.Total() * inputTensor.ElemSize()];
            System.Runtime.InteropServices.Marshal.Copy(inputTensor.Data, inputData, 0, inputData.Length);

            var swInference = Stopwatch.StartNew();
            var outputs = _engine.Run(inputData);
            swInference.Stop();

            var swPostprocess = Stopwatch.StartNew();
            var detections = Postprocess(outputs[0]);
            swPostprocess.Stop();

            var swRender = Stopwatch.StartNew();
            var detectionsScaled = ConvertToOriginalCoordinates(detections);
            DrawDetections(img, detectionsScaled);
            swRender.Stop();

            swTotal.Stop();

            // Print timing summary
            Console.WriteLine("\n[INFO] Performance Summary:");
            Console.WriteLine($"  - Preprocess : {swPreprocess.Elapsed.TotalMilliseconds:F2} ms");
            Console.WriteLine($"  - Inference  : {swInference.Elapsed.TotalMilliseconds:F2} ms");
            Console.WriteLine($"  - Postprocess: {swPostprocess.Elapsed.TotalMilliseconds:F2} ms");
            Console.WriteLine($"  - Render     : {swRender.Elapsed.TotalMilliseconds:F2} ms");
            Console.WriteLine($"  - Total      : {swTotal.Elapsed.TotalMilliseconds:F2} ms");
            Console.WriteLine($"  - Detections : {detections.Count}");

            if (display)
            {
                Cv2.ImShow("Output", img);
                Cv2.WaitKey(0);
                Cv2.DestroyAllWindows();
            }
            else
            {
                string outputPath = Path.Combine(
                    Path.GetDirectoryName(imagePath) ?? ".",
                    $"yolov5_output_{Path.GetFileName(imagePath)}");
                Cv2.ImWrite(outputPath, img);
                Console.WriteLine($"[SUCCESS] Output saved: {outputPath}");
            }

            inputTensor.Dispose();
            img.Dispose();
        }

        /// <summary>
        /// Run inference on video stream.
        /// </summary>
        public void StreamInference(string source, bool display = true)
        {
            VideoCapture cap;
            
            if (int.TryParse(source, out int cameraIndex))
            {
                cap = new VideoCapture(cameraIndex);
                Console.WriteLine($"\n[INFO] Camera index: {cameraIndex}");
            }
            else
            {
                cap = new VideoCapture(source);
                Console.WriteLine($"\n[INFO] Video source: {source}");
            }

            if (!cap.IsOpened())
            {
                Console.WriteLine($"[ERROR] Failed to open input source: {source}");
                return;
            }

            int width = (int)cap.Get(VideoCaptureProperties.FrameWidth);
            int height = (int)cap.Get(VideoCaptureProperties.FrameHeight);
            double fps = cap.Get(VideoCaptureProperties.Fps);
            int totalFrames = (int)cap.Get(VideoCaptureProperties.FrameCount);

            Console.WriteLine($"[INFO] Input source resolution (WxH): {width}x{height}");
            if (totalFrames > 0)
                Console.WriteLine($"[INFO] Total frames: {totalFrames}");
            if (fps > 0)
                Console.WriteLine($"[INFO] Input source FPS: {fps:F2}");

            Console.WriteLine("\n[INFO] Starting inference... Press 'q' or ESC to quit.");

            double sumPreprocess = 0, sumInference = 0, sumPostprocess = 0, sumRender = 0;
            int frameCount = 0;
            var swTotal = Stopwatch.StartNew();

            using var frame = new Mat();
            
            while (true)
            {
                if (!cap.Read(frame) || frame.Empty())
                    break;

                frameCount++;

                var swPreprocess = Stopwatch.StartNew();
                var inputTensor = Preprocess(frame);
                var inputData = new byte[inputTensor.Total() * inputTensor.ElemSize()];
                System.Runtime.InteropServices.Marshal.Copy(inputTensor.Data, inputData, 0, inputData.Length);
                swPreprocess.Stop();

                var swInference = Stopwatch.StartNew();
                var outputs = _engine.Run(inputData);
                swInference.Stop();

                var swPostprocess = Stopwatch.StartNew();
                var detections = Postprocess(outputs[0]);
                swPostprocess.Stop();

                sumPreprocess += swPreprocess.Elapsed.TotalMilliseconds;
                sumInference += swInference.Elapsed.TotalMilliseconds;
                sumPostprocess += swPostprocess.Elapsed.TotalMilliseconds;

                if (display)
                {
                    var swRender = Stopwatch.StartNew();
                    var detectionsScaled = ConvertToOriginalCoordinates(detections);
                    DrawDetections(frame, detectionsScaled);
                    Cv2.ImShow("Output", frame);
                    swRender.Stop();
                    sumRender += swRender.Elapsed.TotalMilliseconds;

                    int key = Cv2.WaitKey(1);
                    if (key == 'q' || key == 27) // 'q' or ESC
                    {
                        Console.WriteLine("\n[INFO] User requested to quit");
                        break;
                    }
                }

                inputTensor.Dispose();
            }

            swTotal.Stop();
            cap.Release();
            Cv2.DestroyAllWindows();

            if (frameCount > 0)
            {
                double elapsed = swTotal.Elapsed.TotalSeconds;
                Console.WriteLine("\n[INFO] Performance Summary:");
                Console.WriteLine($"  - Frames processed: {frameCount}");
                Console.WriteLine($"  - Total time      : {elapsed:F2} s");
                Console.WriteLine($"  - Average FPS     : {frameCount / elapsed:F2}");
                Console.WriteLine($"  - Avg Preprocess  : {sumPreprocess / frameCount:F2} ms");
                Console.WriteLine($"  - Avg Inference   : {sumInference / frameCount:F2} ms");
                Console.WriteLine($"  - Avg Postprocess : {sumPostprocess / frameCount:F2} ms");
                if (display)
                    Console.WriteLine($"  - Avg Render      : {sumRender / frameCount:F2} ms");
            }
        }

        /// <summary>
        /// Dispose resources.
        /// </summary>
        public void Dispose()
        {
            _engine?.Dispose();
        }

        /// <summary>
        /// COCO 80 class labels.
        /// </summary>
        private static string[] GetCocoLabels()
        {
            return new[]
            {
                "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat",
                "traffic light", "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat",
                "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack",
                "umbrella", "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard", "sports ball",
                "kite", "baseball bat", "baseball glove", "skateboard", "surfboard", "tennis racket",
                "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
                "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake",
                "chair", "couch", "potted plant", "bed", "dining table", "toilet", "tv", "laptop",
                "mouse", "remote", "keyboard", "cell phone", "microwave", "oven", "toaster", "sink",
                "refrigerator", "book", "clock", "vase", "scissors", "teddy bear", "hair drier", "toothbrush"
            };
        }
    }

    /// <summary>
    /// Detection result.
    /// </summary>
    public class Detection
    {
        public float X1 { get; set; }
        public float Y1 { get; set; }
        public float X2 { get; set; }
        public float Y2 { get; set; }
        public float Score { get; set; }
        public int ClassId { get; set; }
    }
}
