//
// Copyright (C) 2018- DEEPX Ltd.
// All rights reserved.
//
// This software is the property of DEEPX and is provided exclusively to customers
// who are supplied with DEEPX NPU (Neural Processing Unit).
// Unauthorized sharing or usage is strictly prohibited by law.
//

using System;

namespace DxEngine
{
    /// <summary>
    /// Exception thrown when a DX-RT operation fails.
    /// </summary>
    public class DxEngineException : Exception
    {
        /// <summary>
        /// Gets the error code associated with this exception, if available.
        /// </summary>
        public int? ErrorCode { get; }

        /// <summary>
        /// Initializes a new instance of the DxEngineException class.
        /// </summary>
        public DxEngineException()
            : base()
        {
        }

        /// <summary>
        /// Initializes a new instance of the DxEngineException class with a specified error message.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        public DxEngineException(string message)
            : base(message)
        {
        }

        /// <summary>
        /// Initializes a new instance of the DxEngineException class with a specified error message
        /// and a reference to the inner exception that is the cause of this exception.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        /// <param name="innerException">The exception that is the cause of the current exception.</param>
        public DxEngineException(string message, Exception innerException)
            : base(message, innerException)
        {
        }

        /// <summary>
        /// Initializes a new instance of the DxEngineException class with a specified error message
        /// and error code.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        /// <param name="errorCode">The error code associated with this exception.</param>
        public DxEngineException(string message, int errorCode)
            : base(message)
        {
            ErrorCode = errorCode;
        }

        /// <summary>
        /// Initializes a new instance of the DxEngineException class with a specified error message,
        /// error code, and a reference to the inner exception.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        /// <param name="errorCode">The error code associated with this exception.</param>
        /// <param name="innerException">The exception that is the cause of the current exception.</param>
        public DxEngineException(string message, int errorCode, Exception innerException)
            : base(message, innerException)
        {
            ErrorCode = errorCode;
        }

        /// <summary>
        /// Returns a string representation of the exception.
        /// </summary>
        public override string ToString()
        {
            if (ErrorCode.HasValue)
                return $"{base.ToString()} (ErrorCode: {ErrorCode})";
            return base.ToString();
        }
    }
}
