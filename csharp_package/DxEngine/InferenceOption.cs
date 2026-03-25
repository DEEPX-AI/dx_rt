//
// Copyright (C) 2018- DEEPX Ltd.
// All rights reserved.
//
// This software is the property of DEEPX and is provided exclusively to customers
// who are supplied with DEEPX NPU (Neural Processing Unit).
// Unauthorized sharing or usage is strictly prohibited by law.
//

using System;
using System.Collections.Generic;

namespace DxEngine
{
    /// <summary>
    /// Defines how NPU cores are bound or utilized for inference.
    /// </summary>
    public enum BoundOption
    {
        /// <summary>Use all available NPU cores.</summary>
        NpuAll = 0,
        /// <summary>Use NPU core 0 only.</summary>
        Npu0 = 1,
        /// <summary>Use NPU core 1 only.</summary>
        Npu1 = 2,
        /// <summary>Use NPU core 2 only.</summary>
        Npu2 = 3,
        /// <summary>Use NPU cores 0 and 1.</summary>
        Npu01 = 4,
        /// <summary>Use NPU cores 1 and 2.</summary>
        Npu12 = 5,
        /// <summary>Use NPU cores 0 and 2.</summary>
        Npu02 = 6
    }

    /// <summary>
    /// Configuration options for the InferenceEngine.
    /// This is a pure managed class that holds configuration values.
    /// The actual native InferenceOption is created by the C++/CLI wrapper.
    /// </summary>
    public class InferenceOption
    {
        /// <summary>
        /// Default maximum task load value.
        /// </summary>
        public const int TaskMaxLoadDefault = 6;

        /// <summary>
        /// Maximum allowed task load limit.
        /// </summary>
        public const int TaskMaxLoadLimit = 100;

        /// <summary>
        /// Initializes a new InferenceOption object with default values.
        /// </summary>
        public InferenceOption()
        {
            Devices = new List<int>();
            UseOrt = false;
            BoundOption = BoundOption.NpuAll;
            BufferCount = TaskMaxLoadDefault;
        }

        /// <summary>
        /// Gets or sets whether to use ONNX Runtime for CPU tasks.
        /// </summary>
        public bool UseOrt { get; set; }

        /// <summary>
        /// Gets or sets the NPU core binding option.
        /// </summary>
        public BoundOption BoundOption { get; set; }

        /// <summary>
        /// Gets or sets the list of device IDs to be used for inference.
        /// </summary>
        public List<int> Devices { get; set; }

        /// <summary>
        /// Gets or sets the buffer count for inference.
        /// </summary>
        public int BufferCount { get; set; }

        /// <summary>
        /// Returns a string representation of the InferenceOption.
        /// </summary>
        public override string ToString()
        {
            return $"InferenceOption(UseOrt={UseOrt}, BoundOption={BoundOption}, Devices=[{string.Join(", ", Devices)}], BufferCount={BufferCount})";
        }
    }
}
