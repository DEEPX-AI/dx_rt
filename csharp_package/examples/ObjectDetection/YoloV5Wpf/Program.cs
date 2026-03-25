//
// Copyright (C) 2018- DEEPX Ltd.
// All rights reserved.
//
// YOLOv5 Object Detection using WPF (No External Packages)
// Uses System.Windows.Media.Imaging - built into .NET Windows
//

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using DxEngine;

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
    /// YOLOv5 detector using WPF imaging (no external packages).
    /// </summary>
    public class WpfYoloDetector : IDisposable
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

        // Colors for visualization
        private static readonly Color[] ClassColors;

        static WpfYoloDetector()
        {
            var rng = new Random(42);
            ClassColors = new Color[80];
            for (int i = 0; i < 80; i++)
            {
                ClassColors[i] = Color.FromRgb(
                    (byte)rng.Next(100, 256),
                    (byte)rng.Next(100, 256),
                    (byte)rng.Next(100, 256));
            }
        }

        public WpfYoloDetector(string modelPath, float scoreThreshold = 0.5f, float nmsThreshold = 0.45f)
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

        public static Color GetClassColor(int id) =>
            id >= 0 && id < ClassColors.Length ? ClassColors[id] : Colors.Green;

        /// <summary>
        /// Load image using WPF.
        /// </summary>
        public static BitmapSource LoadImage(string path)
        {
            var bitmap = new BitmapImage();
            bitmap.BeginInit();
            bitmap.UriSource = new Uri(path, UriKind.Absolute);
            bitmap.CacheOption = BitmapCacheOption.OnLoad;
            bitmap.EndInit();
            bitmap.Freeze();

            // Convert to Bgr24 for consistent pixel access
            if (bitmap.Format != PixelFormats.Bgr24)
            {
                return new FormatConvertedBitmap(bitmap, PixelFormats.Bgr24, null, 0);
            }
            return bitmap;
        }

        /// <summary>
        /// Detect objects in image.
        /// </summary>
        public List<Detection> Detect(BitmapSource image)
        {
            int origW = image.PixelWidth, origH = image.PixelHeight;

            // Preprocess
            var inputData = Preprocess(image, out float scale, out int padX, out int padY);

            // Run inference
            var outputs = _engine.Run(inputData);

            if (outputs.Count == 0 || outputs[0].Length == 0)
                return new List<Detection>();

            // Postprocess
            return Postprocess(outputs[0], origW, origH, scale, padX, padY);
        }

        /// <summary>
        /// Preprocess image for YOLO input using WPF imaging.
        /// </summary>
        private byte[] Preprocess(BitmapSource image, out float scale, out int padX, out int padY)
        {
            int origW = image.PixelWidth, origH = image.PixelHeight;

            // Calculate scale to fit in input size while maintaining aspect ratio
            scale = Math.Min((float)_inputWidth / origW, (float)_inputHeight / origH);
            int newW = (int)(origW * scale), newH = (int)(origH * scale);
            padX = (_inputWidth - newW) / 2;
            padY = (_inputHeight - newH) / 2;

            // Resize image using WPF
            var resized = new TransformedBitmap(image, new ScaleTransform(
                (double)newW / origW, (double)newH / origH));

            // Ensure Bgr24 format
            BitmapSource resizedBgr = resized.Format == PixelFormats.Bgr24 
                ? resized 
                : new FormatConvertedBitmap(resized, PixelFormats.Bgr24, null, 0);

            // Get resized pixel data
            int resizedStride = newW * 3;
            var resizedPixels = new byte[newH * resizedStride];
            resizedBgr.CopyPixels(resizedPixels, resizedStride, 0);

            // Create padded tensor with gray background (114, 114, 114)
            var tensor = new float[_inputHeight * _inputWidth * 3];

            // Fill with gray
            for (int i = 0; i < tensor.Length; i += 3)
            {
                tensor[i] = 114f / 255f;     // R
                tensor[i + 1] = 114f / 255f; // G
                tensor[i + 2] = 114f / 255f; // B
            }

            // Copy resized image to center (BGR to RGB + normalize)
            for (int y = 0; y < newH; y++)
            {
                int srcRow = y * resizedStride;
                int dstRow = (y + padY) * _inputWidth * 3 + padX * 3;

                for (int x = 0; x < newW; x++)
                {
                    int srcIdx = srcRow + x * 3;
                    int dstIdx = dstRow + x * 3;

                    // BGR to RGB and normalize to 0-1
                    tensor[dstIdx + 0] = resizedPixels[srcIdx + 2] / 255f; // R
                    tensor[dstIdx + 1] = resizedPixels[srcIdx + 1] / 255f; // G
                    tensor[dstIdx + 2] = resizedPixels[srcIdx + 0] / 255f; // B
                }
            }

            // Convert to bytes
            var bytes = new byte[tensor.Length * sizeof(float)];
            Buffer.BlockCopy(tensor, 0, bytes, 0, bytes.Length);
            return bytes;
        }

        /// <summary>
        /// Postprocess YOLO output.
        /// </summary>
        private List<Detection> Postprocess(byte[] outputData, int origW, int origH,
            float scale, int padX, int padY)
        {
            var output = new float[outputData.Length / sizeof(float)];
            Buffer.BlockCopy(outputData, 0, output, 0, outputData.Length);

            int stride = 5 + _numClasses;
            int numDets = output.Length / stride;

            var boxes = new List<(int x, int y, int w, int h)>();
            var confs = new List<float>();
            var classIds = new List<int>();

            for (int i = 0; i < numDets; i++)
            {
                int off = i * stride;
                float obj = output[off + 4];
                if (obj < _scoreThreshold) continue;

                // Find best class
                int maxId = 0;
                float maxScore = output[off + 5];
                for (int c = 1; c < _numClasses; c++)
                {
                    if (output[off + 5 + c] > maxScore)
                    {
                        maxScore = output[off + 5 + c];
                        maxId = c;
                    }
                }

                float conf = obj * maxScore;
                if (conf < _scoreThreshold) continue;

                // Convert coordinates
                float cx = (output[off + 0] - padX) / scale;
                float cy = (output[off + 1] - padY) / scale;
                float w = output[off + 2] / scale;
                float h = output[off + 3] / scale;

                boxes.Add(((int)(cx - w / 2), (int)(cy - h / 2), (int)w, (int)h));
                confs.Add(conf);
                classIds.Add(maxId);
            }

            // Apply NMS
            var indices = ApplyNMS(boxes, confs, _nmsThreshold);

            return indices.Select(i => new Detection
            {
                X1 = Math.Max(0, boxes[i].x),
                Y1 = Math.Max(0, boxes[i].y),
                X2 = Math.Min(origW, boxes[i].x + boxes[i].w),
                Y2 = Math.Min(origH, boxes[i].y + boxes[i].h),
                Score = confs[i],
                ClassId = classIds[i]
            }).ToList();
        }

        private List<int> ApplyNMS(List<(int x, int y, int w, int h)> boxes, List<float> scores, float threshold)
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

        private float ComputeIoU((int x, int y, int w, int h) a, (int x, int y, int w, int h) b)
        {
            int x1 = Math.Max(a.x, b.x), y1 = Math.Max(a.y, b.y);
            int x2 = Math.Min(a.x + a.w, b.x + b.w);
            int y2 = Math.Min(a.y + a.h, b.y + b.h);
            int inter = Math.Max(0, x2 - x1) * Math.Max(0, y2 - y1);
            int union = a.w * a.h + b.w * b.h - inter;
            return union > 0 ? (float)inter / union : 0;
        }

        public (double latencyMs, double npuMs) GetTiming() =>
            (_engine.GetLatency() / 1000.0, _engine.GetNpuInferenceTime() / 1000.0);

        public void Dispose()
        {
            if (!_disposed) { _engine?.Dispose(); _disposed = true; }
        }
    }

    /// <summary>
    /// WPF-based image processing utilities.
    /// </summary>
    public static class WpfImageUtils
    {
        /// <summary>
        /// Draw detections on image using WPF DrawingVisual.
        /// </summary>
        public static BitmapSource DrawDetections(BitmapSource image, List<Detection> detections)
        {
            // Create DrawingVisual for rendering
            var visual = new DrawingVisual();
            using (var dc = visual.RenderOpen())
            {
                // Draw original image
                dc.DrawImage(image, new Rect(0, 0, image.PixelWidth, image.PixelHeight));

                // Draw each detection
                foreach (var det in detections)
                {
                    // Skip invalid boxes
                    int width = det.X2 - det.X1;
                    int height = det.Y2 - det.Y1;
                    if (width <= 0 || height <= 0) continue;

                    var color = WpfYoloDetector.GetClassColor(det.ClassId);
                    var pen = new Pen(new SolidColorBrush(color), 2);
                    pen.Freeze();

                    // Draw bounding box
                    var rect = new Rect(det.X1, det.Y1, width, height);
                    dc.DrawRectangle(null, pen, rect);

                    // Draw label
                    var label = $"{WpfYoloDetector.GetClassName(det.ClassId)} {det.Score:F2}";
                    var text = new FormattedText(
                        label,
                        CultureInfo.InvariantCulture,
                        FlowDirection.LeftToRight,
                        new Typeface("Arial"),
                        12,
                        Brushes.White,
                        1.0);

                    // Label background
                    double labelY = det.Y1 - text.Height - 2;
                    if (labelY < 0) labelY = det.Y1 + 2;

                    var bgBrush = new SolidColorBrush(Color.FromArgb(180, color.R, color.G, color.B));
                    bgBrush.Freeze();
                    dc.DrawRectangle(bgBrush, null, new Rect(det.X1, labelY, text.Width + 4, text.Height));
                    dc.DrawText(text, new Point(det.X1 + 2, labelY));
                }
            }

            // Render to bitmap
            var rtb = new RenderTargetBitmap(
                image.PixelWidth, image.PixelHeight,
                96, 96, PixelFormats.Pbgra32);
            rtb.Render(visual);
            rtb.Freeze();

            return rtb;
        }

        /// <summary>
        /// Save image to file using WPF encoders.
        /// </summary>
        public static void SaveImage(BitmapSource image, string path)
        {
            BitmapEncoder encoder;
            var ext = Path.GetExtension(path).ToLower();
            
            encoder = ext switch
            {
                ".png" => new PngBitmapEncoder(),
                ".bmp" => new BmpBitmapEncoder(),
                ".gif" => new GifBitmapEncoder(),
                ".tiff" or ".tif" => new TiffBitmapEncoder(),
                _ => new JpegBitmapEncoder { QualityLevel = 95 }
            };

            encoder.Frames.Add(BitmapFrame.Create(image));
            using var fs = new FileStream(path, FileMode.Create);
            encoder.Save(fs);
        }
    }

    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Console.WriteLine("=== YOLOv5 Object Detection (WPF - No External Packages) ===");
            Console.WriteLine("Uses System.Windows.Media.Imaging - built into .NET Windows");
            Console.WriteLine();

            if (!NativeBridge.IsAvailable)
            {
                Console.WriteLine($"[ERROR] {NativeBridge.LoadError}");
                return;
            }
            Console.WriteLine($"[INFO] DX-RT v{NativeBridge.GetVersion()}");

            // Parse args
            string? model = null, image = null, output = null;
            int iterations = 1;

            for (int i = 0; i < args.Length; i++)
            {
                if ((args[i] == "-m" || args[i] == "--model") && i + 1 < args.Length) model = args[++i];
                else if ((args[i] == "-i" || args[i] == "--image") && i + 1 < args.Length) image = args[++i];
                else if ((args[i] == "-o" || args[i] == "--output") && i + 1 < args.Length) output = args[++i];
                else if ((args[i] == "-n" || args[i] == "--iterations") && i + 1 < args.Length) int.TryParse(args[++i], out iterations);
            }

            if (model == null || image == null)
            {
                PrintHelp();
                return;
            }

            try
            {
                ProcessImage(model, image, output, iterations);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[ERROR] {ex.Message}");
                Console.WriteLine(ex.StackTrace);
            }
        }

        static void ProcessImage(string modelPath, string imagePath, string? outputPath, int iterations)
        {
            Console.WriteLine($"\n[Processing] {imagePath}");

            if (!File.Exists(imagePath))
            {
                Console.WriteLine("[ERROR] Image not found");
                return;
            }

            using var detector = new WpfYoloDetector(modelPath);

            // Load image using WPF
            var image = WpfYoloDetector.LoadImage(imagePath);
            Console.WriteLine($"[INFO] Image: {image.PixelWidth}x{image.PixelHeight}");

            // Warmup
            detector.Detect(image);

            // Benchmark
            var sw = Stopwatch.StartNew();
            List<Detection> detections = null!;
            for (int i = 0; i < iterations; i++)
            {
                detections = detector.Detect(image);
            }
            sw.Stop();

            var (latencyMs, npuMs) = detector.GetTiming();
            double avgTime = sw.ElapsedMilliseconds / (double)iterations;
            double fps = 1000.0 / avgTime;

            Console.WriteLine($"\n[Result] {detections.Count} objects detected");
            Console.WriteLine($"[Performance] Iterations: {iterations}");
            Console.WriteLine($"  - Avg time: {avgTime:F2} ms ({fps:F1} FPS)");
            Console.WriteLine($"  - Last inference: {latencyMs:F2} ms (NPU: {npuMs:F2} ms)");

            // Draw detections
            var resultImage = WpfImageUtils.DrawDetections(image, detections);

            // Save output
            outputPath ??= Path.Combine(
                Path.GetDirectoryName(imagePath) ?? ".",
                $"wpf_output_{Path.GetFileName(imagePath)}");
            WpfImageUtils.SaveImage(resultImage, outputPath);
            Console.WriteLine($"[Saved] {outputPath}");

            // Print detections
            Console.WriteLine("\n[Detections]");
            foreach (var d in detections)
            {
                Console.WriteLine($"  - {WpfYoloDetector.GetClassName(d.ClassId)}: {d.Score:F2} " +
                                  $"({d.X1},{d.Y1})-({d.X2},{d.Y2})");
            }
        }

        static void PrintHelp()
        {
            Console.WriteLine("YOLOv5 Object Detection (WPF - No External Packages)");
            Console.WriteLine();
            Console.WriteLine("Uses built-in WPF imaging APIs (System.Windows.Media.Imaging).");
            Console.WriteLine("No NuGet packages required!");
            Console.WriteLine();
            Console.WriteLine("Usage:");
            Console.WriteLine("  YoloV5Wpf -m <model.dxnn> -i <image.jpg> [-o <output.jpg>] [-n <iterations>]");
            Console.WriteLine();
            Console.WriteLine("Options:");
            Console.WriteLine("  -m, --model       Path to DXNN model file (required)");
            Console.WriteLine("  -i, --image       Path to input image (required)");
            Console.WriteLine("  -o, --output      Path to output image");
            Console.WriteLine("  -n, --iterations  Number of iterations for benchmark (default: 1)");
            Console.WriteLine();
            Console.WriteLine("Note: Video processing not supported (WPF doesn't have native video APIs).");
            Console.WriteLine("      For video, use OpenCvSharp or FFmpeg separately.");
        }
    }
}
