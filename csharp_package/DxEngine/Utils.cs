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
using System.Runtime.InteropServices;

namespace DxEngine
{
    /// <summary>
    /// Utility methods for the DX-RT engine.
    /// </summary>
    public static class Utils
    {
        /// <summary>
        /// Parses a model file and displays information about it.
        /// </summary>
        /// <param name="modelPath">Path to the .dxnn model file.</param>
        /// <returns>0 if successful, -1 if failed.</returns>
        /// <exception cref="FileNotFoundException">Thrown when the model file is not found.</exception>
        public static int ParseModel(string modelPath)
        {
            if (string.IsNullOrEmpty(modelPath))
                throw new ArgumentException("Model path cannot be null or empty.", nameof(modelPath));

            if (!File.Exists(modelPath))
                throw new FileNotFoundException($"Model file not found: {modelPath}", modelPath);

            return NativeMethods.ParseModel(modelPath);
        }

        /// <summary>
        /// Converts a managed byte array to an unmanaged memory block.
        /// </summary>
        /// <param name="data">The byte array to convert.</param>
        /// <returns>A pointer to the unmanaged memory.</returns>
        /// <remarks>The caller is responsible for freeing the memory using Marshal.FreeHGlobal.</remarks>
        public static IntPtr ToUnmanagedMemory(byte[] data)
        {
            if (data == null || data.Length == 0)
                return IntPtr.Zero;

            IntPtr ptr = Marshal.AllocHGlobal(data.Length);
            Marshal.Copy(data, 0, ptr, data.Length);
            return ptr;
        }

        /// <summary>
        /// Copies data from unmanaged memory to a managed float array.
        /// </summary>
        /// <param name="source">Pointer to the source unmanaged memory.</param>
        /// <param name="length">Number of float elements to copy.</param>
        /// <returns>A new float array containing the copied data.</returns>
        public static float[] CopyFromUnmanaged(IntPtr source, int length)
        {
            if (source == IntPtr.Zero || length <= 0)
                return Array.Empty<float>();

            float[] result = new float[length];
            Marshal.Copy(source, result, 0, length);
            return result;
        }
    }
}
