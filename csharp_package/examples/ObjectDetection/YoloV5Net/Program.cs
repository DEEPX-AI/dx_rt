//
// Copyright (C) 2018- DEEPX Ltd.
// All rights reserved.
//
// YOLOv5 Minimal Example - Simple inference without video display
// Uses OpenCvSharp only for image I/O (no video streaming)
//

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using DxEngine;
using OpenCvSharp;

namespace DxEngine.Examples
{
    /// <summary>
    /// Detection result.
    /// </summary>
    public class Detection
    {
        public int X1 { get; set; }
        public int Y1 { get; set; }
        public int X2 { get; set; }
        public int Y2 { get; set; }
        public float Score { get; set; }
        public int ClassId { get; set; }
    }

    /// <summary>
    /// Simple YOLOv5 detector - minimal implementation.
    /// </summary>
    public class SimpleYoloDetector : IDisposable
    {
        private readonly InferenceEngineNative _engine;
        private readonly int _inputWidth;
        private readonly int _inputHeight;
        private readonly float _scoreThreshold;
        private readonly float _nmsThreshold;
        private readonly int _numClasses;
        private bool _disposed;

        // COCO 80 class names
        private static readonly string[] ClassNames = {
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

        public SimpleYoloDetector(string modelPath, float scoreThreshold = 0.5f, float nmsThreshold = 0.45f)
        {
            _scoreThreshold = scoreThreshold;
            _nmsThreshold = nmsThreshold;
            _numClasses = 80;

            var option = new InferenceOption
            {
                UseOrt = false,
                BoundOption = BoundOption.NpuAll,
                BufferCount = 6
            };

            _engine = new InferenceEngineNative(modelPath, option);

            // Get input dimensions
            var inputInfo = _engine.InputTensorInfo.FirstOrDefault();
            if (inputInfo?.Shape.Length >= 4)
            {
                _inputHeight = inputInfo.Shape[1];
                _inputWidth = inputInfo.Shape[2];
            }
            else
            {
                _inputWidth = _inputHeight = 640;
            }

            Console.WriteLine($"[Model] Input size: {_inputWidth}x{_inputHeight}");
        }

        public static string GetClassName(int id) => 
            id >= 0 && id < ClassNames.Length ? ClassNames[id] : $"class_{id}";

        /// <summary>
        /// Detect objects in image.
        /// </summary>
        public List<Detection> Detect(Mat image)
        {
            int origW = image.Width, origH = image.Height;
            
            // Preprocess: resize with letterbox
            float scale = Math.Min((float)_inputWidth / origW, (float)_inputHeight / origH);
            int newW = (int)(origW * scale), newH = (int)(origH * scale);
            int padX = (_inputWidth - newW) / 2, padY = (_inputHeight - newH) / 2;

            using var resized = new Mat();
            Cv2.Resize(image, resized, new Size(newW, newH));

            using var padded = new Mat(_inputHeight, _inputWidth, MatType.CV_8UC3, new Scalar(114, 114, 114));
            resized.CopyTo(padded[new Rect(padX, padY, newW, newH)]);

            // Convert to float tensor (NHWC, 0-1) - using unsafe pointer access
            var tensor = new float[_inputHeight * _inputWidth * 3];
            unsafe
            {
                byte* ptr = (byte*)padded.Data.ToPointer();
                int stride = (int)padded.Step();
                int idx = 0;
                for (int y = 0; y < _inputHeight; y++)
                {
                    byte* row = ptr + y * stride;
                    for (int x = 0; x < _inputWidth; x++)
                    {
                        // BGR to RGB and normalize
                        tensor[idx++] = row[x * 3 + 2] / 255.0f; // R
                        tensor[idx++] = row[x * 3 + 1] / 255.0f; // G
                        tensor[idx++] = row[x * 3 + 0] / 255.0f; // B
                    }
                }
            }

            // Run inference
            var inputBytes = new byte[tensor.Length * sizeof(float)];
            Buffer.BlockCopy(tensor, 0, inputBytes, 0, inputBytes.Length);
            var outputs = _engine.Run(inputBytes);

            if (outputs.Count == 0 || outputs[0].Length == 0)
                return new List<Detection>();

            // Postprocess
            return Postprocess(outputs[0], origW, origH, scale, padX, padY);
        }

        private List<Detection> Postprocess(byte[] outputData, int origW, int origH, 
            float scale, int padX, int padY)
        {
            var output = new float[outputData.Length / sizeof(float)];
            Buffer.BlockCopy(outputData, 0, output, 0, outputData.Length);

            int stride = 5 + _numClasses;
            int numDets = output.Length / stride;

            var boxes = new List<Rect>();
            var confs = new List<float>();
            var classIds = new List<int>();

            for (int i = 0; i < numDets; i++)
            {
                int off = i * stride;
                float obj = output[off + 4];
                if (obj < _scoreThreshold) continue;

                // Find best class
                int maxId = 0; float maxScore = output[off + 5];
                for (int c = 1; c < _numClasses; c++)
                {
                    if (output[off + 5 + c] > maxScore) { maxScore = output[off + 5 + c]; maxId = c; }
                }

                float conf = obj * maxScore;
                if (conf < _scoreThreshold) continue;

                // Convert coordinates
                float cx = (output[off + 0] - padX) / scale;
                float cy = (output[off + 1] - padY) / scale;
                float w = output[off + 2] / scale;
                float h = output[off + 3] / scale;

                boxes.Add(new Rect((int)(cx - w/2), (int)(cy - h/2), (int)w, (int)h));
                confs.Add(conf);
                classIds.Add(maxId);
            }

            // NMS
            var indices = ApplyNMS(boxes, confs, _nmsThreshold);

            return indices.Select(i => new Detection
            {
                X1 = Math.Max(0, boxes[i].X),
                Y1 = Math.Max(0, boxes[i].Y),
                X2 = Math.Min(origW, boxes[i].X + boxes[i].Width),
                Y2 = Math.Min(origH, boxes[i].Y + boxes[i].Height),
                Score = confs[i],
                ClassId = classIds[i]
            }).ToList();
        }

        private List<int> ApplyNMS(List<Rect> boxes, List<float> scores, float threshold)
        {
            var indices = new List<int>();
            var sorted = Enumerable.Range(0, scores.Count).OrderByDescending(i => scores[i]).ToList();
            var suppressed = new bool[boxes.Count];

            foreach (int i in sorted)
            {
                if (suppressed[i]) continue;
                indices.Add(i);
                for (int j = 0; j < boxes.Count; j++)
                {
                    if (!suppressed[j] && i != j && ComputeIoU(boxes[i], boxes[j]) > threshold)
                        suppressed[j] = true;
                }
            }
            return indices;
        }

        private float ComputeIoU(Rect a, Rect b)
        {
            int x1 = Math.Max(a.X, b.X), y1 = Math.Max(a.Y, b.Y);
            int x2 = Math.Min(a.X + a.Width, b.X + b.Width);
            int y2 = Math.Min(a.Y + a.Height, b.Y + b.Height);
            int inter = Math.Max(0, x2 - x1) * Math.Max(0, y2 - y1);
            int union = a.Width * a.Height + b.Width * b.Height - inter;
            return union > 0 ? (float)inter / union : 0;
        }

        public (double latencyMs, double npuMs) GetTiming() =>
            (_engine.GetLatency() / 1000.0, _engine.GetNpuInferenceTime() / 1000.0);

        public void Dispose()
        {
            if (!_disposed) { _engine?.Dispose(); _disposed = true; }
        }
    }

    class Program
    {
        static readonly Random Rng = new Random(42);
        static readonly Scalar[] Colors = Enumerable.Range(0, 80)
            .Select(_ => new Scalar(Rng.Next(100, 256), Rng.Next(100, 256), Rng.Next(100, 256)))
            .ToArray();

        static void Main(string[] args)
        {
            Console.WriteLine("=== YOLOv5 Minimal Example ===");
            
            if (!NativeBridge.IsAvailable)
            {
                Console.WriteLine($"[ERROR] {NativeBridge.LoadError}");
                return;
            }
            Console.WriteLine($"[INFO] DX-RT v{NativeBridge.GetVersion()}");

            // Parse args
            string? model = null, image = null, video = null, output = null;
            for (int i = 0; i < args.Length; i++)
            {
                if ((args[i] == "-m" || args[i] == "--model") && i + 1 < args.Length) model = args[++i];
                else if ((args[i] == "-i" || args[i] == "--image") && i + 1 < args.Length) image = args[++i];
                else if ((args[i] == "-v" || args[i] == "--video") && i + 1 < args.Length) video = args[++i];
                else if ((args[i] == "-o" || args[i] == "--output") && i + 1 < args.Length) output = args[++i];
            }

            if (model == null) { PrintHelp(); return; }

            try
            {
                using var detector = new SimpleYoloDetector(model);
                
                if (image != null) ProcessImage(detector, image, output);
                else if (video != null) ProcessVideo(detector, video, output);
                else PrintHelp();
            }
            catch (Exception ex) { Console.WriteLine($"[ERROR] {ex.Message}"); }
        }

        static void ProcessImage(SimpleYoloDetector detector, string path, string? output)
        {
            Console.WriteLine($"\n[Processing] {path}");
            using var img = Cv2.ImRead(path);
            if (img.Empty()) { Console.WriteLine("[ERROR] Cannot read image"); return; }

            var dets = detector.Detect(img);
            DrawDetections(img, dets);

            var (lat, npu) = detector.GetTiming();
            Console.WriteLine($"[Result] {dets.Count} objects, {lat:F1}ms (NPU: {npu:F1}ms)");

            output ??= Path.Combine(Path.GetDirectoryName(path) ?? ".", $"output_{Path.GetFileName(path)}");
            Cv2.ImWrite(output, img);
            Console.WriteLine($"[Saved] {output}");

            foreach (var d in dets)
                Console.WriteLine($"  - {SimpleYoloDetector.GetClassName(d.ClassId)}: {d.Score:F2}");
        }

        static void ProcessVideo(SimpleYoloDetector detector, string path, string? output)
        {
            Console.WriteLine($"\n[Processing] {path}");
            using var cap = new VideoCapture(path);
            if (!cap.IsOpened()) { Console.WriteLine("[ERROR] Cannot open video"); return; }

            double fps = cap.Fps;
            int total = (int)cap.FrameCount;
            Console.WriteLine($"[Video] {cap.FrameWidth}x{cap.FrameHeight} @ {fps:F1}fps, {total} frames");

            // Use MJPEG codec (widely supported) with .avi output
            output ??= Path.Combine(Path.GetDirectoryName(path) ?? ".", $"output_{Path.GetFileNameWithoutExtension(path)}.avi");
            using var writer = new VideoWriter(output, FourCC.MJPG, fps, 
                new Size(cap.FrameWidth, cap.FrameHeight));

            var sw = Stopwatch.StartNew();
            var latencies = new List<double>();
            int count = 0;

            using var frame = new Mat();
            while (cap.Read(frame))
            {
                var dets = detector.Detect(frame);
                latencies.Add(detector.GetTiming().latencyMs);
                DrawDetections(frame, dets);
                writer.Write(frame);

                if (++count % 100 == 0)
                    Console.WriteLine($"  {count}/{total} ({100.0 * count / total:F0}%)");
            }

            sw.Stop();
            Console.WriteLine($"\n[Done] {count} frames in {sw.Elapsed.TotalSeconds:F1}s");
            Console.WriteLine($"[FPS] {count / sw.Elapsed.TotalSeconds:F1} (Avg inference: {latencies.Average():F1}ms)");
            Console.WriteLine($"[Saved] {output}");
        }

        static void DrawDetections(Mat img, List<Detection> dets)
        {
            foreach (var d in dets)
            {
                var color = Colors[d.ClassId % Colors.Length];
                Cv2.Rectangle(img, new Point(d.X1, d.Y1), new Point(d.X2, d.Y2), color, 2);
                
                var label = $"{SimpleYoloDetector.GetClassName(d.ClassId)} {d.Score:P0}";
                var size = Cv2.GetTextSize(label, HersheyFonts.HersheySimplex, 0.5, 1, out int baseline);
                int y = d.Y1 - 5 > size.Height ? d.Y1 - 5 : d.Y1 + size.Height + 5;
                Cv2.Rectangle(img, new Point(d.X1, y - size.Height - 3), 
                    new Point(d.X1 + size.Width, y + 3), color, -1);
                Cv2.PutText(img, label, new Point(d.X1, y), 
                    HersheyFonts.HersheySimplex, 0.5, Scalar.White, 1);
            }
        }

        static void PrintHelp()
        {
            Console.WriteLine("Usage: YoloV5Net -m <model.dxnn> [-i <image> | -v <video>] [-o <output>]");
        }
    }
}
