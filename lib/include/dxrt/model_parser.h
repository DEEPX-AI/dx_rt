/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */
 
#pragma once

#include "dxrt/common.h"
#include "dxrt/model.h"
#include <memory>
#include <string>

namespace dxrt {

/**
 * @brief Abstract base class for version-specific model parsers
 * 
 * This interface defines the contract for parsing different versions of DXNN files.
 * Each version (v6, v7, etc.) will have its own concrete implementation.
 */
class IModelParser {
public:
    virtual ~IModelParser() = default;
    
    /**
     * @brief Parse the model file and populate ModelDataBase
     * @param filePath Path to the DXNN file
     * @param modelData Output parameter to store parsed data
     * @return Compile type string (e.g., "release", "debug")
     */
    virtual std::string ParseModel(const std::string& filePath, ModelDataBase& modelData) = 0;


    /**
     * @brief Parse the model from memory buffer and populate ModelDataBase
     * @param modelBuffer Pointer to the DXNN file data in memory
     * @param modelSize Size of the DXNN file data
     * @param modelData Output parameter to store parsed data
     * @return Compile type string (e.g., "release", "debug")
     */
    virtual std::string ParseModel(const uint8_t* modelBuffer, size_t modelSize, ModelDataBase& modelData) = 0;
    
    /**
     * @brief Get the version number this parser supports
     * @return Version number (6, 7, etc.)
     */
    virtual int GetSupportedVersion() const = 0;
    
    /**
     * @brief Get the name of this parser
     * @return Parser name (e.g., "DXNN V6 Parser", "DXNN V7 Parser")
     */
    virtual std::string GetParserName() const = 0;

    /**
     * @brief Set the number of buffers to use during parsing
     * @param bufferCount Number of buffers
     */
    void SetTaskBufferCount(int bufferCount)
    {
        _taskBufferCount = bufferCount;
    }

protected:
    int _taskBufferCount = DXRT_TASK_MAX_LOAD_VALUE;
};

/**
 * @brief Factory class for creating version-specific model parsers
 * 
 * This factory automatically detects the DXNN file version and creates
 * the appropriate parser instance.
 */
class ModelParserFactory {
public:
    /**
     * @brief Create a parser for the specified file
     * @param filePath Path to the DXNN file
     * @return Unique pointer to the appropriate parser
     * @throws InvalidModelException if version is not supported
     */
    static std::unique_ptr<IModelParser> CreateParser(const std::string& filePath);


    /**
     * @brief Create a parser for the specified memory buffer
     * @param modelBuffer Pointer to the DXNN file data in memory
     * @param modelSize Size of the DXNN file data
     * @return Unique pointer to the appropriate parser
     * @throws InvalidModelException if version is not supported
     */
    static std::unique_ptr<IModelParser> CreateParser(const uint8_t* modelBuffer, size_t modelSize);
    
    /**
     * @brief Create a parser for a specific version
     * @param version DXNN file format version
     * @return Unique pointer to the appropriate parser
     * @throws InvalidModelException if version is not supported
     */
    static std::unique_ptr<IModelParser> CreateParser(int version);
    
    /**
     * @brief Get the file format version from a DXNN file
     * @param filePath Path to the DXNN file
     * @return Version number
     * @throws FileNotFoundException if file doesn't exist
     * @throws InvalidModelException if file format is invalid
     */
    static int GetFileFormatVersion(const std::string& filePath);

    /**
     * @brief Get the file format version from a DXNN file buffer
     * @param modelBuffer Pointer to the DXNN file data in memory
     * @param modelSize Size of the DXNN file data
     * @return Version number
     * @throws InvalidModelException if file format is invalid
     */
    static int GetFileFormatVersion(const uint8_t* modelBuffer, size_t modelSize);
    
    /**
     * @brief Check if a version is supported
     * @param version Version number to check
     * @return true if supported, false otherwise
     */
    static bool IsVersionSupported(int version);
    
    /**
     * @brief Get list of supported versions
     * @return Vector of supported version numbers
     */
    static std::vector<int> GetSupportedVersions();

private:
    // Prevent instantiation
    ModelParserFactory() = delete;
};

} // namespace dxrt 