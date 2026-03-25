//
// Copyright (C) 2018- DEEPX Ltd.
// All rights reserved.
//
// This software is the property of DEEPX and is provided exclusively to customers
// who are supplied with DEEPX NPU (Neural Processing Unit).
// Unauthorized sharing or usage is strictly prohibited by law.
//

using System;

namespace DxEngine.Cli
{
    /// <summary>
    /// Main entry point for DxEngine CLI tools.
    /// </summary>
    public class Program
    {
        /// <summary>
        /// Main entry point.
        /// </summary>
        /// <param name="args">Command line arguments.</param>
        /// <returns>Exit code.</returns>
        public static int Main(string[] args)
        {
            if (args.Length == 0)
            {
                PrintUsage();
                return 1;
            }

            string command = args[0].ToLower();
            string[] commandArgs = args.Length > 1 ? args[1..] : Array.Empty<string>();

            return command switch
            {
                "parse" or "parse_model" => ParseModel.Main(commandArgs),
                "run" or "run_model" => RunModel.Main(commandArgs),
                "-h" or "--help" or "help" => ShowHelp(),
                "-v" or "--version" or "version" => ShowVersion(),
                _ => UnknownCommand(command)
            };
        }

        private static void PrintUsage()
        {
            Console.WriteLine("DxEngine CLI - Command-line tools for DEEPX NPU Inference Engine");
            Console.WriteLine();
            Console.WriteLine("Usage: dxcli <command> [options]");
            Console.WriteLine();
            Console.WriteLine("Commands:");
            Console.WriteLine("  parse, parse_model    Parse and display model information");
            Console.WriteLine("  run, run_model        Run model inference with benchmarking");
            Console.WriteLine("  help                  Show this help message");
            Console.WriteLine("  version               Show version information");
            Console.WriteLine();
            Console.WriteLine("Use 'dxcli <command> --help' for more information about a command.");
        }

        private static int ShowHelp()
        {
            PrintUsage();
            return 0;
        }

        private static int ShowVersion()
        {
            Console.WriteLine($"DxEngine CLI version {DxEngine.Version.Current}");
            return 0;
        }

        private static int UnknownCommand(string command)
        {
            Console.Error.WriteLine($"Unknown command: {command}");
            Console.Error.WriteLine();
            PrintUsage();
            return 1;
        }
    }
}
