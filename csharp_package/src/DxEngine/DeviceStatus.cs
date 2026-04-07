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
    /// Provides access to device status information for DEEPX NPU devices.
    /// </summary>
    public class DeviceStatus : IDisposable
    {
        private IntPtr _handle;
        private bool _disposed = false;

        /// <summary>
        /// Private constructor - use GetCurrentStatus factory method.
        /// </summary>
        private DeviceStatus(IntPtr handle)
        {
            _handle = handle;
        }

        /// <summary>
        /// Gets the current status of the specified device.
        /// </summary>
        /// <param name="deviceId">The device ID to query.</param>
        /// <returns>A DeviceStatus instance for the specified device.</returns>
        /// <exception cref="DxEngineException">Thrown when device status cannot be retrieved.</exception>
        public static DeviceStatus GetCurrentStatus(int deviceId)
        {
            IntPtr handle = NativeMethods.GetDeviceStatus(deviceId);
            if (handle == IntPtr.Zero)
                throw new DxEngineException($"Failed to get device status for device {deviceId}");
            
            return new DeviceStatus(handle);
        }

        /// <summary>
        /// Gets the total number of available devices.
        /// </summary>
        /// <returns>The number of available DEEPX NPU devices.</returns>
        public static int GetDeviceCount()
        {
            return NativeMethods.GetDeviceCount();
        }

        /// <summary>
        /// Gets the temperature of the specified channel.
        /// </summary>
        /// <param name="channel">The temperature channel index.</param>
        /// <returns>The temperature value in degrees Celsius.</returns>
        public int GetTemperature(int channel)
        {
            ThrowIfDisposed();
            return NativeMethods.GetDeviceTemperature(_handle, channel);
        }

        /// <summary>
        /// Gets the device ID.
        /// </summary>
        /// <returns>The device ID.</returns>
        public int GetId()
        {
            ThrowIfDisposed();
            return NativeMethods.GetDeviceId(_handle);
        }

        /// <summary>
        /// Gets the NPU voltage of the specified channel.
        /// </summary>
        /// <param name="channel">The voltage channel index.</param>
        /// <returns>The voltage value in millivolts.</returns>
        public int GetNpuVoltage(int channel)
        {
            ThrowIfDisposed();
            return NativeMethods.GetDeviceNpuVoltage(_handle, channel);
        }

        /// <summary>
        /// Gets the NPU clock of the specified channel.
        /// </summary>
        /// <param name="channel">The clock channel index.</param>
        /// <returns>The clock frequency in MHz.</returns>
        public int GetNpuClock(int channel)
        {
            ThrowIfDisposed();
            return NativeMethods.GetDeviceNpuClock(_handle, channel);
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(DeviceStatus));
        }

        /// <summary>
        /// Releases all resources used by the DeviceStatus.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Releases the unmanaged resources used by the DeviceStatus.
        /// </summary>
        /// <param name="disposing">True if called from Dispose(), false if called from finalizer.</param>
        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (_handle != IntPtr.Zero)
                {
                    NativeMethods.ReleaseDeviceStatus(_handle);
                    _handle = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        /// <summary>
        /// Finalizer for DeviceStatus.
        /// </summary>
        ~DeviceStatus()
        {
            Dispose(false);
        }
    }
}
