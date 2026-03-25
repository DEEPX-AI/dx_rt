//
// Copyright (C) 2018- DEEPX Ltd.
// All rights reserved.
//
// This software is the property of DEEPX and is provided exclusively to customers
// who are supplied with DEEPX NPU (Neural Processing Unit).
// Unauthorized sharing or usage is strictly prohibited by law.
//

using System;
using System.IO;
using DxEngine;

namespace DxEngine.Examples.ObjectDetection
{
    /// <summary>
    /// YOLOv5 Object Detection Example - Synchronous Mode
    /// C# port of yolov5_sync.py
    /// 
    /// Usage:
    ///   YoloV5Example.exe --model model.dxnn --image input.jpg
    ///   YoloV5Example.exe --model model.dxnn --video input.mp4
    ///   YoloV5Example.exe --model model.dxnn --camera 0
    /// </summary>
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("=== YOLOv5 Object Detection (C#) ===");

            // Check native library
            if (!NativeBridge.IsAvailable)
            {
                Console.WriteLine($"[ERROR] Native library not available: {NativeBridge.LoadError}");
                return;
            }

            Console.WriteLine($"[INFO] DX-RT Version: {NativeBridge.GetVersion()}");

            // Parse arguments
            var options = ParseArguments(args);
            if (options == null)
            {
                PrintHelp();
                return;
            }

            // Validate model path
            if (!File.Exists(options.ModelPath))
            {
                Console.WriteLine($"[ERROR] Model file not found: {options.ModelPath}");
                return;
            }

            try
            {
                var yolo = new YoloV5(options.ModelPath);

                if (!string.IsNullOrEmpty(options.ImagePath))
                {
                    if (!File.Exists(options.ImagePath))
                    {
                        Console.WriteLine($"[ERROR] Image file not found: {options.ImagePath}");
                        return;
                    }
                    yolo.ImageInference(options.ImagePath, options.Display);
                }
                else if (!string.IsNullOrEmpty(options.VideoPath))
                {
                    if (!File.Exists(options.VideoPath))
                    {
                        Console.WriteLine($"[ERROR] Video file not found: {options.VideoPath}");
                        return;
                    }
                    yolo.StreamInference(options.VideoPath, options.Display);
                }
                else if (options.CameraIndex >= 0)
                {
                    yolo.StreamInference(options.CameraIndex.ToString(), options.Display);
                }
                else if (!string.IsNullOrEmpty(options.RtspUrl))
                {
                    yolo.StreamInference(options.RtspUrl, options.Display);
                }

                yolo.Dispose();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[ERROR] {ex.Message}");
                if (ex.InnerException != null)
                    Console.WriteLine($"  Inner: {ex.InnerException.Message}");
            }
        }

        static Options? ParseArguments(string[] args)
        {
            var options = new Options();

            for (int i = 0; i < args.Length; i++)
            {
                switch (args[i].ToLower())
                {
                    case "--model":
                    case "-m":
                        if (i + 1 < args.Length)
                            options.ModelPath = args[++i];
                        break;
                    case "--image":
                    case "-i":
                        if (i + 1 < args.Length)
                            options.ImagePath = args[++i];
                        break;
                    case "--video":
                    case "-v":
                        if (i + 1 < args.Length)
                            options.VideoPath = args[++i];
                        break;
                    case "--camera":
                    case "-c":
                        if (i + 1 < args.Length && int.TryParse(args[++i], out int camIdx))
                            options.CameraIndex = camIdx;
                        break;
                    case "--rtsp":
                    case "-r":
                        if (i + 1 < args.Length)
                            options.RtspUrl = args[++i];
                        break;
                    case "--no-display":
                        options.Display = false;
                        break;
                    case "--help":
                    case "-h":
                        return null;
                }
            }

            if (string.IsNullOrEmpty(options.ModelPath))
            {
                Console.WriteLine("[ERROR] --model is required");
                return null;
            }

            bool hasInput = !string.IsNullOrEmpty(options.ImagePath) ||
                           !string.IsNullOrEmpty(options.VideoPath) ||
                           options.CameraIndex >= 0 ||
                           !string.IsNullOrEmpty(options.RtspUrl);

            if (!hasInput)
            {
                Console.WriteLine("[ERROR] One of --image, --video, --camera, or --rtsp is required");
                return null;
            }

            return options;
        }

        static void PrintHelp()
        {
            Console.WriteLine(@"
YOLOv5 Object Detection Example

Usage:
  YoloV5Example.exe --model <model.dxnn> --image <input.jpg>
  YoloV5Example.exe --model <model.dxnn> --video <input.mp4>
  YoloV5Example.exe --model <model.dxnn> --camera <index>
  YoloV5Example.exe --model <model.dxnn> --rtsp <url>

Options:
  --model, -m <path>    Path to DXNN model file (required)
  --image, -i <path>    Path to input image
  --video, -v <path>    Path to input video file
  --camera, -c <index>  Camera device index (e.g., 0)
  --rtsp, -r <url>      RTSP stream URL
  --no-display          Do not display output window
  --help, -h            Show this help message

Examples:
  YoloV5Example.exe --model yolov5s.dxnn --image bus.jpg
  YoloV5Example.exe --model yolov5s.dxnn --video traffic.mp4 --no-display
  YoloV5Example.exe --model yolov5s.dxnn --camera 0
");
        }

        class Options
        {
            public string ModelPath { get; set; } = "";
            public string ImagePath { get; set; } = "";
            public string VideoPath { get; set; } = "";
            public int CameraIndex { get; set; } = -1;
            public string RtspUrl { get; set; } = "";
            public bool Display { get; set; } = true;
        }
    }
}
