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

namespace DxEngine.Cli
{
    /// <summary>
    /// CLI tool for parsing and displaying model information.
    /// </summary>
    public class ParseModel
    {
        private readonly ParseOptions _options;

        /// <summary>
        /// Options for parse model command.
        /// </summary>
        public class ParseOptions
        {
            /// <summary>Model file path (required).</summary>
            public string ModelPath { get; set; } = string.Empty;
            
            /// <summary>Show detailed task dependencies and memory usage.</summary>
            public bool Verbose { get; set; } = false;
            
            /// <summary>Save the raw console output to a file (without color codes).</summary>
            public string? OutputFile { get; set; } = null;
            
            /// <summary>Extract JSON binary data (graph_info, rmap_info) to files.</summary>
            public bool JsonExtract { get; set; } = false;
            
            /// <summary>Disable color output.</summary>
            public bool NoColor { get; set; } = false;
        }

        public ParseModel(ParseOptions options)
        {
            _options = options ?? throw new ArgumentNullException(nameof(options));
        }

        /// <summary>
        /// Execute the parse model command.
        /// </summary>
        /// <returns>Exit code (0 for success).</returns>
        public int Execute()
        {
            try
            {
                if (string.IsNullOrEmpty(_options.ModelPath))
                {
                    Console.Error.WriteLine("Error: Model path is required.");
                    return 1;
                }

                if (!File.Exists(_options.ModelPath))
                {
                    Console.Error.WriteLine($"Error: Model path '{_options.ModelPath}' does not exist.");
                    return 1;
                }

                // Use native ParseModel function which outputs to console directly
                return NativeBridge.ParseModel(
                    _options.ModelPath, 
                    _options.Verbose, 
                    _options.JsonExtract, 
                    _options.OutputFile);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine($"Error: {ex.Message}");
                return 1;
            }
        }

        private void PrintHeader(TextWriter output, string title, bool noColor)
        {
            string separator = new string('=', title.Length + 4);
            
            if (!noColor && !Console.IsOutputRedirected)
            {
                Console.ForegroundColor = ConsoleColor.Cyan;
            }
            
            output.WriteLine(separator);
            output.WriteLine($"  {title}");
            output.WriteLine(separator);
            
            if (!noColor && !Console.IsOutputRedirected)
            {
                Console.ResetColor();
            }
        }

        private void PrintTensorInfo(TextWriter output, TensorInfo info, bool verbose, bool noColor)
        {
            string shapeStr = info.Shape != null ? $"[{string.Join(", ", info.Shape)}]" : "[]";
            
            if (!noColor && !Console.IsOutputRedirected)
            {
                Console.ForegroundColor = ConsoleColor.Yellow;
            }
            
            output.WriteLine($"  [{info.Index}] {info.Name}");
            
            if (!noColor && !Console.IsOutputRedirected)
            {
                Console.ResetColor();
            }
            
            output.WriteLine($"      Shape: {shapeStr}");
            output.WriteLine($"      DataType: {info.DataType}");
            
            if (verbose)
            {
                output.WriteLine($"      Elements: {info.ElementCount}");
                output.WriteLine($"      Size: {info.SizeInBytes} bytes");
            }
        }

        /// <summary>
        /// Parse command line arguments and create ParseOptions.
        /// </summary>
        public static ParseOptions ParseArguments(string[] args)
        {
            var options = new ParseOptions();
            
            for (int i = 0; i < args.Length; i++)
            {
                switch (args[i].ToLower())
                {
                    case "-m":
                    case "--model":
                        if (i + 1 < args.Length)
                            options.ModelPath = args[++i];
                        break;
                    case "-v":
                    case "--verbose":
                        options.Verbose = true;
                        break;
                    case "-o":
                    case "--output":
                        if (i + 1 < args.Length)
                            options.OutputFile = args[++i];
                        break;
                    case "-j":
                    case "--json":
                        options.JsonExtract = true;
                        break;
                    case "--no-color":
                        options.NoColor = true;
                        break;
                    case "-h":
                    case "--help":
                        PrintHelp();
                        Environment.Exit(0);
                        break;
                }
            }

            return options;
        }

        /// <summary>
        /// Print help message.
        /// </summary>
        public static void PrintHelp()
        {
            Console.WriteLine("parse_model - Parse and display model information");
            Console.WriteLine();
            Console.WriteLine("Usage: parse_model -m <model_path> [options]");
            Console.WriteLine();
            Console.WriteLine("Options:");
            Console.WriteLine("  -m, --model <path>    Model path (required)");
            Console.WriteLine("  -v, --verbose         Show detailed task dependencies and memory usage");
            Console.WriteLine("  -o, --output <file>   Save the raw console output to a file");
            Console.WriteLine("  -j, --json            Extract JSON binary data to files");
            Console.WriteLine("  --no-color            Disable color output");
            Console.WriteLine("  -h, --help            Show this help message");
            Console.WriteLine();
            Console.WriteLine("Examples:");
            Console.WriteLine("  parse_model -m model.dxnn");
            Console.WriteLine("  parse_model -m model.dxnn -v");
            Console.WriteLine("  parse_model -m model.dxnn -o output.txt");
            Console.WriteLine("  parse_model -m model.dxnn -j");
        }

        /// <summary>
        /// Entry point when called as a subcommand.
        /// </summary>
        public static int Main(string[] args)
        {
            if (args.Length == 0)
            {
                PrintHelp();
                return 1;
            }

            var options = ParseArguments(args);
            var parser = new ParseModel(options);
            return parser.Execute();
        }
    }
}
