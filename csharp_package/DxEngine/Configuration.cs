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
    /// Configuration items for the DX-RT system.
    /// </summary>
    public enum ConfigurationItem
    {
        /// <summary>Debug mode configuration.</summary>
        Debug = 1,
        /// <summary>Profiler configuration.</summary>
        Profiler = 2,
        /// <summary>Service configuration.</summary>
        Service = 3,
        /// <summary>Dynamic CPU thread configuration.</summary>
        DynamicCpuThread = 4,
        /// <summary>Task flow configuration.</summary>
        TaskFlow = 5,
        /// <summary>Show throttling information.</summary>
        ShowThrottling = 6,
        /// <summary>Show profile information.</summary>
        ShowProfile = 7,
        /// <summary>Show model information.</summary>
        ShowModelInfo = 8,
        /// <summary>Custom intra-op threads configuration.</summary>
        CustomIntraOpThreads = 9,
        /// <summary>Custom inter-op threads configuration.</summary>
        CustomInterOpThreads = 10
    }

    /// <summary>
    /// Configuration attributes for configuration items.
    /// </summary>
    public enum ConfigurationAttribute
    {
        /// <summary>Profiler show data attribute.</summary>
        ProfilerShowData = 1001,
        /// <summary>Profiler save data attribute.</summary>
        ProfilerSaveData = 1002,
        /// <summary>Custom intra-op threads number attribute.</summary>
        CustomIntraOpThreadsNum = 1003,
        /// <summary>Custom inter-op threads number attribute.</summary>
        CustomInterOpThreadsNum = 1004
    }

    /// <summary>
    /// Provides configuration management for the DX-RT system.
    /// This class follows the singleton pattern.
    /// </summary>
    public class Configuration : IDisposable
    {
        private static Configuration? _instance;
        private static readonly object _lock = new object();
        private IntPtr _handle;
        private bool _disposed = false;

        /// <summary>
        /// Private constructor for singleton pattern.
        /// </summary>
        private Configuration()
        {
            _handle = NativeMethods.GetConfigurationInstance();
        }

        /// <summary>
        /// Gets the singleton instance of Configuration.
        /// </summary>
        public static Configuration Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_lock)
                    {
                        _instance ??= new Configuration();
                    }
                }
                return _instance;
            }
        }

        /// <summary>
        /// Loads configuration from a file.
        /// </summary>
        /// <param name="fileName">The configuration file path.</param>
        /// <returns>True if successful, false otherwise.</returns>
        /// <exception cref="ArgumentException">Thrown when fileName is null or empty.</exception>
        public bool LoadConfigFile(string fileName)
        {
            ThrowIfDisposed();
            if (string.IsNullOrEmpty(fileName))
                throw new ArgumentException("File name must be a non-empty string.", nameof(fileName));
            
            return NativeMethods.ConfigurationLoadConfigFile(_handle, fileName);
        }

        /// <summary>
        /// Enables or disables a configuration item.
        /// </summary>
        /// <param name="item">The configuration item to set.</param>
        /// <param name="enabled">True to enable, false to disable.</param>
        public void SetEnable(ConfigurationItem item, bool enabled)
        {
            ThrowIfDisposed();
            NativeMethods.ConfigurationSetEnable(_handle, (int)item, enabled);
        }

        /// <summary>
        /// Gets whether a configuration item is enabled.
        /// </summary>
        /// <param name="item">The configuration item to query.</param>
        /// <returns>True if enabled, false otherwise.</returns>
        public bool GetEnable(ConfigurationItem item)
        {
            ThrowIfDisposed();
            return NativeMethods.ConfigurationGetEnable(_handle, (int)item);
        }

        /// <summary>
        /// Sets an attribute value for a configuration item.
        /// </summary>
        /// <param name="item">The configuration item.</param>
        /// <param name="attribute">The attribute to set.</param>
        /// <param name="value">The attribute value.</param>
        public void SetAttribute(ConfigurationItem item, ConfigurationAttribute attribute, string value)
        {
            ThrowIfDisposed();
            NativeMethods.ConfigurationSetAttribute(_handle, (int)item, (int)attribute, value);
        }

        /// <summary>
        /// Gets an attribute value for a configuration item.
        /// </summary>
        /// <param name="item">The configuration item.</param>
        /// <param name="attribute">The attribute to get.</param>
        /// <returns>The attribute value as a string.</returns>
        public string GetAttribute(ConfigurationItem item, ConfigurationAttribute attribute)
        {
            ThrowIfDisposed();
            IntPtr ptr = NativeMethods.ConfigurationGetAttribute(_handle, (int)item, (int)attribute);
            return Marshal.PtrToStringAnsi(ptr) ?? string.Empty;
        }

        /// <summary>
        /// Gets the DX-RT version string.
        /// </summary>
        /// <returns>The version string.</returns>
        public string GetVersion()
        {
            ThrowIfDisposed();
            IntPtr ptr = NativeMethods.ConfigurationGetVersion(_handle);
            return Marshal.PtrToStringAnsi(ptr) ?? string.Empty;
        }

        /// <summary>
        /// Gets the driver version string.
        /// </summary>
        /// <returns>The driver version string.</returns>
        public string GetDriverVersion()
        {
            ThrowIfDisposed();
            IntPtr ptr = NativeMethods.ConfigurationGetDriverVersion(_handle);
            return Marshal.PtrToStringAnsi(ptr) ?? string.Empty;
        }

        /// <summary>
        /// Gets the PCIe driver version string.
        /// </summary>
        /// <returns>The PCIe driver version string.</returns>
        public string GetPcieDriverVersion()
        {
            ThrowIfDisposed();
            IntPtr ptr = NativeMethods.ConfigurationGetPcieDriverVersion(_handle);
            return Marshal.PtrToStringAnsi(ptr) ?? string.Empty;
        }

        /// <summary>
        /// Sets firmware configuration from a JSON file.
        /// </summary>
        /// <param name="jsonFile">The JSON configuration file path.</param>
        public void SetFwConfigWithJson(string jsonFile)
        {
            ThrowIfDisposed();
            if (string.IsNullOrEmpty(jsonFile))
                throw new ArgumentException("JSON file path must be a non-empty string.", nameof(jsonFile));
            
            NativeMethods.ConfigurationSetFwConfigWithJson(_handle, jsonFile);
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(Configuration));
        }

        /// <summary>
        /// Releases all resources used by the Configuration.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Releases the unmanaged resources used by the Configuration.
        /// </summary>
        /// <param name="disposing">True if called from Dispose(), false if called from finalizer.</param>
        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                // Note: Configuration is typically a singleton and may not need explicit cleanup
                _handle = IntPtr.Zero;
                _disposed = true;
            }
        }

        /// <summary>
        /// Finalizer for Configuration.
        /// </summary>
        ~Configuration()
        {
            Dispose(false);
        }
    }
}
