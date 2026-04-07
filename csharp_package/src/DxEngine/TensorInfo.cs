//
// Copyright (C) 2018- DEEPX Ltd.
// All rights reserved.
//
// This software is the property of DEEPX and is provided exclusively to customers
// who are supplied with DEEPX NPU (Neural Processing Unit).
// Unauthorized sharing or usage is strictly prohibited by law.
//

using System;
using System.Linq;

namespace DxEngine
{
    /// <summary>
    /// Contains information about a tensor (input or output).
    /// </summary>
    public class TensorInfo
    {
        /// <summary>
        /// Gets or sets the tensor index.
        /// </summary>
        public int Index { get; set; }

        /// <summary>
        /// Gets or sets the tensor name.
        /// </summary>
        public string Name { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the tensor shape (dimensions).
        /// </summary>
        public int[] Shape { get; set; } = Array.Empty<int>();

        /// <summary>
        /// Gets or sets the tensor data type.
        /// </summary>
        public DataType DataType { get; set; } = DataType.None;

        /// <summary>
        /// Gets the total number of elements in the tensor.
        /// </summary>
        public long ElementCount
        {
            get
            {
                if (Shape == null || Shape.Length == 0)
                    return 0;
                
                return Shape.Aggregate(1L, (acc, dim) => acc * dim);
            }
        }

        /// <summary>
        /// Gets the total size in bytes of the tensor.
        /// </summary>
        public long SizeInBytes
        {
            get
            {
                try
                {
                    return ElementCount * DataTypeMapper.GetSizeInBytes(DataType);
                }
                catch
                {
                    return 0;
                }
            }
        }

        /// <summary>
        /// Returns a string representation of the TensorInfo.
        /// </summary>
        public override string ToString()
        {
            string shapeStr = Shape != null ? $"[{string.Join(", ", Shape)}]" : "[]";
            return $"TensorInfo(Index={Index}, Name='{Name}', Shape={shapeStr}, DataType={DataType})";
        }
    }
}
