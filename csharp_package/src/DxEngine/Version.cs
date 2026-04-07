//
// Copyright (C) 2018- DEEPX Ltd.
// All rights reserved.
//
// This software is the property of DEEPX and is provided exclusively to customers
// who are supplied with DEEPX NPU (Neural Processing Unit).
// Unauthorized sharing or usage is strictly prohibited by law.
//

using System.Reflection;

namespace DxEngine
{
    /// <summary>
    /// Provides version information for the DxEngine library.
    /// </summary>
    public static class Version
    {
        private static readonly System.Version _version =
            Assembly.GetExecutingAssembly().GetName().Version ?? new System.Version(0, 0, 0);

        /// <summary>
        /// Gets the current version of the DxEngine library.
        /// </summary>
        public static string Current => $"{_version.Major}.{_version.Minor}.{_version.Build}";

        /// <summary>
        /// Gets the major version number.
        /// </summary>
        public static int Major => _version.Major;

        /// <summary>
        /// Gets the minor version number.
        /// </summary>
        public static int Minor => _version.Minor;

        /// <summary>
        /// Gets the patch version number.
        /// </summary>
        public static int Patch => _version.Build;
    }
}
