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
    /// Supported data types for tensors.
    /// </summary>
    public enum DataType
    {
        /// <summary>No type specified.</summary>
        None = 0,
        /// <summary>Unsigned 8-bit integer.</summary>
        UInt8 = 1,
        /// <summary>Unsigned 16-bit integer.</summary>
        UInt16 = 2,
        /// <summary>Unsigned 32-bit integer.</summary>
        UInt32 = 3,
        /// <summary>Unsigned 64-bit integer.</summary>
        UInt64 = 4,
        /// <summary>Signed 8-bit integer.</summary>
        Int8 = 5,
        /// <summary>Signed 16-bit integer.</summary>
        Int16 = 6,
        /// <summary>Signed 32-bit integer.</summary>
        Int32 = 7,
        /// <summary>Signed 64-bit integer.</summary>
        Int64 = 8,
        /// <summary>32-bit floating point.</summary>
        Float32 = 9,
        /// <summary>64-bit floating point.</summary>
        Float64 = 10,
        /// <summary>Bounding box output type.</summary>
        BBox = 100,
        /// <summary>Face detection output type.</summary>
        Face = 101,
        /// <summary>Pose estimation output type.</summary>
        Pose = 102
    }

    /// <summary>
    /// Provides utility methods for data type conversions.
    /// </summary>
    public static class DataTypeMapper
    {
        /// <summary>
        /// Converts a string representation to a DataType enum value.
        /// </summary>
        /// <param name="typeString">The data type string (case-insensitive).</param>
        /// <returns>The corresponding DataType value.</returns>
        /// <exception cref="ArgumentException">Thrown when the type string is not recognized.</exception>
        public static DataType FromString(string typeString)
        {
            if (string.IsNullOrEmpty(typeString))
                throw new ArgumentException("Type string cannot be null or empty.", nameof(typeString));

            return typeString.ToUpperInvariant() switch
            {
                "UINT8" => DataType.UInt8,
                "UINT16" => DataType.UInt16,
                "UINT32" => DataType.UInt32,
                "UINT64" => DataType.UInt64,
                "INT8" => DataType.Int8,
                "INT16" => DataType.Int16,
                "INT32" => DataType.Int32,
                "INT64" => DataType.Int64,
                "FLOAT" or "FLOAT32" => DataType.Float32,
                "DOUBLE" or "FLOAT64" => DataType.Float64,
                "BBOX" => DataType.BBox,
                "FACE" => DataType.Face,
                "POSE" => DataType.Pose,
                _ => throw new ArgumentException($"Unknown data type string: {typeString}", nameof(typeString))
            };
        }

        /// <summary>
        /// Gets the size in bytes for a given data type.
        /// </summary>
        /// <param name="dataType">The data type.</param>
        /// <returns>The size in bytes.</returns>
        /// <exception cref="ArgumentException">Thrown when the data type has no fixed size.</exception>
        public static int GetSizeInBytes(DataType dataType)
        {
            return dataType switch
            {
                DataType.UInt8 or DataType.Int8 => 1,
                DataType.UInt16 or DataType.Int16 => 2,
                DataType.UInt32 or DataType.Int32 or DataType.Float32 => 4,
                DataType.UInt64 or DataType.Int64 or DataType.Float64 => 8,
                _ => throw new ArgumentException($"Data type {dataType} has no fixed size.", nameof(dataType))
            };
        }

        /// <summary>
        /// Gets the corresponding .NET Type for a given DataType.
        /// </summary>
        /// <param name="dataType">The data type.</param>
        /// <returns>The corresponding .NET Type.</returns>
        /// <exception cref="ArgumentException">Thrown when the data type has no .NET equivalent.</exception>
        public static Type ToSystemType(DataType dataType)
        {
            return dataType switch
            {
                DataType.UInt8 => typeof(byte),
                DataType.UInt16 => typeof(ushort),
                DataType.UInt32 => typeof(uint),
                DataType.UInt64 => typeof(ulong),
                DataType.Int8 => typeof(sbyte),
                DataType.Int16 => typeof(short),
                DataType.Int32 => typeof(int),
                DataType.Int64 => typeof(long),
                DataType.Float32 => typeof(float),
                DataType.Float64 => typeof(double),
                _ => throw new ArgumentException($"Data type {dataType} has no .NET equivalent.", nameof(dataType))
            };
        }
    }
}
