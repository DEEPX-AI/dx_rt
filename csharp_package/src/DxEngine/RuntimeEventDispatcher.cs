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
    /// Event severity levels for categorizing runtime events.
    /// </summary>
    public enum EventLevel
    {
        /// <summary>Informational messages for normal operation events.</summary>
        Info = 1,
        /// <summary>Warning messages for potential issues that don't stop execution.</summary>
        Warning = 2,
        /// <summary>Error messages for recoverable failures.</summary>
        Error = 3,
        /// <summary>Critical errors that may cause system instability.</summary>
        Critical = 4
    }

    /// <summary>
    /// Event type categories for classifying the source of events.
    /// </summary>
    public enum EventType
    {
        /// <summary>Events related to NPU core operations.</summary>
        DeviceCore = 1000,
        /// <summary>Device status change events.</summary>
        DeviceStatus = 1001,
        /// <summary>Input/Output operation events.</summary>
        DeviceIO = 1002,
        /// <summary>Memory management events.</summary>
        DeviceMemory = 1003,
        /// <summary>Unknown or unclassified event types.</summary>
        Unknown = 1004
    }

    /// <summary>
    /// Specific event codes for identifying the exact nature of events.
    /// </summary>
    public enum EventCode
    {
        /// <summary>Input data write operation event.</summary>
        WriteInput = 2000,
        /// <summary>Output data read operation event.</summary>
        ReadOutput = 2001,
        /// <summary>Memory overflow or capacity exceeded.</summary>
        MemoryOverflow = 2002,
        /// <summary>Memory allocation failure or issue.</summary>
        MemoryAllocation = 2003,
        /// <summary>General device event notification.</summary>
        DeviceEvent = 2004,
        /// <summary>Device recovery action taken.</summary>
        RecoveryOccurred = 2005,
        /// <summary>Operation timeout event.</summary>
        TimeoutOccurred = 2006,
        /// <summary>Device throttling notification.</summary>
        ThrottlingNotice = 2007,
        /// <summary>Device throttling emergency notification.</summary>
        ThrottlingEmergency = 2008,
        /// <summary>Unknown or unclassified event code.</summary>
        Unknown = 2009
    }

    /// <summary>
    /// Event arguments for runtime events.
    /// </summary>
    public class RuntimeEventArgs : EventArgs
    {
        /// <summary>Gets the event severity level.</summary>
        public EventLevel Level { get; }
        
        /// <summary>Gets the event type category.</summary>
        public EventType Type { get; }
        
        /// <summary>Gets the specific event code.</summary>
        public EventCode Code { get; }
        
        /// <summary>Gets the event message.</summary>
        public string Message { get; }
        
        /// <summary>Gets the event timestamp.</summary>
        public string Timestamp { get; }

        /// <summary>
        /// Initializes a new RuntimeEventArgs instance.
        /// </summary>
        public RuntimeEventArgs(EventLevel level, EventType type, EventCode code, string message, string timestamp)
        {
            Level = level;
            Type = type;
            Code = code;
            Message = message;
            Timestamp = timestamp;
        }
    }

    /// <summary>
    /// Delegate for handling runtime events.
    /// </summary>
    /// <param name="sender">The event sender.</param>
    /// <param name="e">The event arguments.</param>
    public delegate void RuntimeEventHandler(object? sender, RuntimeEventArgs e);

    /// <summary>
    /// Singleton class for dispatching and handling runtime events from the DX-RT system.
    /// This class provides a centralized event dispatching mechanism for runtime events
    /// such as device errors, warnings, and notifications.
    /// </summary>
    public class RuntimeEventDispatcher : IDisposable
    {
        private static RuntimeEventDispatcher? _instance;
        private static readonly object _lock = new object();
        private IntPtr _handle;
        private bool _disposed = false;
        
        // Native callback delegate - must be kept alive
        private NativeMethods.NativeEventCallback? _nativeCallback;

        /// <summary>
        /// Event raised when a runtime event occurs.
        /// </summary>
        public event RuntimeEventHandler? EventReceived;

        /// <summary>
        /// Private constructor for singleton pattern.
        /// </summary>
        private RuntimeEventDispatcher()
        {
            _handle = NativeMethods.GetRuntimeEventDispatcherInstance();
        }

        /// <summary>
        /// Gets the singleton instance of RuntimeEventDispatcher.
        /// </summary>
        public static RuntimeEventDispatcher Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_lock)
                    {
                        _instance ??= new RuntimeEventDispatcher();
                    }
                }
                return _instance;
            }
        }

        /// <summary>
        /// Dispatches a runtime event with specified parameters.
        /// </summary>
        /// <param name="level">Severity level of the event.</param>
        /// <param name="type">Category of the event.</param>
        /// <param name="code">Specific event code.</param>
        /// <param name="eventMessage">Descriptive message providing event details.</param>
        /// <exception cref="ArgumentException">Thrown when eventMessage is null or empty.</exception>
        public void DispatchEvent(EventLevel level, EventType type, EventCode code, string eventMessage)
        {
            ThrowIfDisposed();
            if (string.IsNullOrEmpty(eventMessage))
                throw new ArgumentException("Event message must be a non-empty string.", nameof(eventMessage));

            NativeMethods.RuntimeEventDispatcherDispatchEvent(
                _handle,
                (int)level,
                (int)type,
                (int)code,
                eventMessage);
        }

        /// <summary>
        /// Registers a custom event handler callback.
        /// </summary>
        /// <remarks>
        /// Only one handler can be registered at a time; subsequent calls will replace
        /// the previous handler.
        /// </remarks>
        public void RegisterEventHandler()
        {
            ThrowIfDisposed();

            // Create and store the native callback to prevent garbage collection
            _nativeCallback = OnNativeEventReceived;
            NativeMethods.RuntimeEventDispatcherRegisterHandler(_handle, _nativeCallback);
        }

        /// <summary>
        /// Unregisters the current event handler.
        /// </summary>
        public void UnregisterEventHandler()
        {
            ThrowIfDisposed();
            NativeMethods.RuntimeEventDispatcherUnregisterHandler(_handle);
            _nativeCallback = null;
        }

        /// <summary>
        /// Sets the current level threshold for event filtering.
        /// </summary>
        /// <param name="level">The minimum severity level for events to be processed.</param>
        public void SetCurrentLevel(EventLevel level)
        {
            ThrowIfDisposed();
            NativeMethods.RuntimeEventDispatcherSetCurrentLevel(_handle, (int)level);
        }

        /// <summary>
        /// Gets the current level threshold for event filtering.
        /// </summary>
        /// <returns>The current minimum severity level.</returns>
        public EventLevel GetCurrentLevel()
        {
            ThrowIfDisposed();
            return (EventLevel)NativeMethods.RuntimeEventDispatcherGetCurrentLevel(_handle);
        }

        private void OnNativeEventReceived(int level, int type, int code, string message, string timestamp)
        {
            var args = new RuntimeEventArgs(
                (EventLevel)level,
                (EventType)type,
                (EventCode)code,
                message,
                timestamp);

            EventReceived?.Invoke(this, args);
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(RuntimeEventDispatcher));
        }

        /// <summary>
        /// Releases all resources used by the RuntimeEventDispatcher.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Releases the unmanaged resources used by the RuntimeEventDispatcher.
        /// </summary>
        /// <param name="disposing">True if called from Dispose(), false if called from finalizer.</param>
        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    _nativeCallback = null;
                }
                _handle = IntPtr.Zero;
                _disposed = true;
            }
        }

        /// <summary>
        /// Finalizer for RuntimeEventDispatcher.
        /// </summary>
        ~RuntimeEventDispatcher()
        {
            Dispose(false);
        }
    }
}
