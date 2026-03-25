//
// Copyright (C) 2018- DEEPX Ltd.
// All rights reserved.
//
// This software is the property of DEEPX and is provided exclusively to customers
// who are supplied with DEEPX NPU (Neural Processing Unit).
// Unauthorized sharing or usage is strictly prohibited by law.
//

using System;
using System.Runtime.InteropServices;

namespace DxEngine
{
    /// <summary>
    /// Native method declarations for P/Invoke to the DX-RT native library.
    /// </summary>
    internal static class NativeMethods
    {
        private const string LibraryName = "dxrt";

        // ============================================================
        // InferenceEngine
        // ============================================================

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateInferenceEngine(
            [MarshalAs(UnmanagedType.LPStr)] string modelPath,
            IntPtr optionHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateInferenceEngineFromBuffer(
            IntPtr buffer,
            int bufferSize,
            IntPtr optionHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DestroyInferenceEngine(IntPtr engineHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetInputCount(IntPtr engineHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetOutputCount(IntPtr engineHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int RunInference(
            IntPtr engineHandle,
            IntPtr[] inputPtrs,
            int[] inputSizes,
            int inputCount,
            IntPtr[] outputPtrs,
            int[] outputSizes,
            int outputCount);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        public static extern string GetTensorName(IntPtr engineHandle, int index, bool isInput);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetTensorShapePtr(IntPtr engineHandle, int index, bool isInput, out int dimCount);

        public static int[] GetTensorShape(IntPtr engineHandle, int index, bool isInput)
        {
            IntPtr shapePtr = GetTensorShapePtr(engineHandle, index, isInput, out int dimCount);
            if (shapePtr == IntPtr.Zero || dimCount <= 0)
                return Array.Empty<int>();

            int[] shape = new int[dimCount];
            Marshal.Copy(shapePtr, shape, 0, dimCount);
            return shape;
        }

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern DataType GetTensorDataType(IntPtr engineHandle, int index, bool isInput);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetInputSize(IntPtr engineHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetOutputSize(IntPtr engineHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern double GetLatency(IntPtr engineHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern double GetNpuInferenceTime(IntPtr engineHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern double GetLatencyMean(IntPtr engineHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern double GetNpuInferenceTimeMean(IntPtr engineHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern double RunBenchmark(
            IntPtr engineHandle,
            int loops,
            IntPtr[] inputPtrs,
            int[] inputSizes,
            int inputCount);

        // ============================================================
        // InferenceOption
        // ============================================================

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateInferenceOption();

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DestroyInferenceOption(IntPtr optionHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool GetInferenceOptionUseOrt(IntPtr optionHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetInferenceOptionUseOrt(IntPtr optionHandle, [MarshalAs(UnmanagedType.I1)] bool value);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetInferenceOptionBoundOption(IntPtr optionHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetInferenceOptionBoundOption(IntPtr optionHandle, int value);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetInferenceOptionBufferCount(IntPtr optionHandle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetInferenceOptionBufferCount(IntPtr optionHandle, int value);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetInferenceOptionDevices(IntPtr optionHandle, int[] devices, int count);

        // ============================================================
        // DeviceStatus
        // ============================================================

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetDeviceStatus(int deviceId);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ReleaseDeviceStatus(IntPtr handle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetDeviceCount();

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetDeviceTemperature(IntPtr handle, int channel);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetDeviceId(IntPtr handle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetDeviceNpuVoltage(IntPtr handle, int channel);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetDeviceNpuClock(IntPtr handle, int channel);

        // ============================================================
        // Configuration
        // ============================================================

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetConfigurationInstance();

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool ConfigurationLoadConfigFile(
            IntPtr handle,
            [MarshalAs(UnmanagedType.LPStr)] string fileName);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ConfigurationSetEnable(IntPtr handle, int item, [MarshalAs(UnmanagedType.I1)] bool enabled);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool ConfigurationGetEnable(IntPtr handle, int item);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ConfigurationSetAttribute(
            IntPtr handle,
            int item,
            int attribute,
            [MarshalAs(UnmanagedType.LPStr)] string value);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ConfigurationGetAttribute(IntPtr handle, int item, int attribute);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ConfigurationGetVersion(IntPtr handle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ConfigurationGetDriverVersion(IntPtr handle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ConfigurationGetPcieDriverVersion(IntPtr handle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ConfigurationSetFwConfigWithJson(
            IntPtr handle,
            [MarshalAs(UnmanagedType.LPStr)] string jsonFile);

        // ============================================================
        // RuntimeEventDispatcher
        // ============================================================

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void NativeEventCallback(
            int level,
            int type,
            int code,
            [MarshalAs(UnmanagedType.LPStr)] string message,
            [MarshalAs(UnmanagedType.LPStr)] string timestamp);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetRuntimeEventDispatcherInstance();

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void RuntimeEventDispatcherDispatchEvent(
            IntPtr handle,
            int level,
            int type,
            int code,
            [MarshalAs(UnmanagedType.LPStr)] string eventMessage);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void RuntimeEventDispatcherRegisterHandler(
            IntPtr handle,
            NativeEventCallback callback);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void RuntimeEventDispatcherUnregisterHandler(IntPtr handle);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void RuntimeEventDispatcherSetCurrentLevel(IntPtr handle, int level);

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int RuntimeEventDispatcherGetCurrentLevel(IntPtr handle);

        // ============================================================
        // Utility Functions
        // ============================================================

        [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int ParseModel(
            [MarshalAs(UnmanagedType.LPStr)] string modelPath);
    }
}
