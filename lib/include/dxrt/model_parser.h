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
class DXRT_API IModelParser {
public:
    virtual ~IModelParser() = default;
    
    /**
     * @brief Parse the model file and populate ModelDataBase
     * @param filePath Path to the DXNN file
     * @param modelData Output parameter to store parsed data
     * @return Compile type string (e.g., "release", "debug")
     * @throws FileNotFoundException if file doesn't exist or can't be read
     */
    std::string ParseModel(const std::string& filePath, ModelDataBase& modelData) const;

    /**
     * @brief Parse the model from memory buffer and populate ModelDataBase
     * 
     * This is a template method that:
     * 1. Calls loadBinaryInfo() to parse header and metadata
     * 2. Calls PreProcessModel() for version-specific transformations
     * 3. Calls loadGraphInfo() to parse graph structure
     * 4. Calls loadRmapInfo() to parse register mapping
     * 
     * @param modelBuffer Pointer to the DXNN file data in memory
     * @param modelSize Size of the DXNN file data
     * @param modelData Output parameter to store parsed data
     * @return Compile type string (e.g., "release", "debug")
     */
    std::string ParseModel(const uint8_t* modelBuffer, size_t modelSize, ModelDataBase& modelData) const;
    
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

    /**
     * @brief Get the number of buffers to use during parsing
     * @return Number of buffers
     */
    int GetTaskBufferCount() const
    {
        return _taskBufferCount;
    }

protected:
    /**
     * @brief Load binary information from the model file header
     * 
     * Parses the 8KB header section containing metadata, offsets, and sizes
     * for all binary sections (rmap, weight, graph_info, etc.)
     * 
     * @param param Output parameter for binary info database
     * @param buffer File buffer containing DXNN data
     * @param fileSize Total size of the file
     * @return DXNN file format version number
     * @throws InvalidModelException if file format is invalid
     */
    virtual int loadBinaryInfo(deepx_binaryinfo::BinaryInfoDatabase& param, 
                               const char* buffer, 
                               int fileSize) const = 0;

    /**
     * @brief Load graph information from the model data
     * 
     * Parses the graph_info JSON section to extract model structure,
     * input/output tensors, subgraphs, and offloading information.
     * 
     * @param param Output parameter for graph info database
     * @param data Model data containing binary info with graph_info string
     * @return 0 on success, -1 on failure
     * @throws InvalidModelException if graph_info is malformed
     */
    virtual int loadGraphInfo(deepx_graphinfo::GraphInfoDatabase& param, 
                              ModelDataBase& data) const = 0;

    /**
     * @brief Load rmap information from the model data
     * 
     * Parses the rmap_info JSON section(s) to extract register mapping,
     * tensor metadata, memory layout, and NPU configuration.
     * 
     * @param param Output parameter for rmap info database
     * @param data Model data containing binary info with rmap_info strings
     * @return Model compile type string (e.g., "release", "debug")
     * @throws InvalidModelException if rmap_info is malformed
     */
    virtual std::string loadRmapInfo(deepx_rmapinfo::rmapInfoDatabase& param, 
                                      ModelDataBase& data) const = 0;

    /**
     * @brief Pre-process model data after loadBinaryInfo but before loadGraphInfo
     * 
     * This hook allows version-specific transformations. For example:
     * - V6: Convert V6 format to V7 format (graph_info and rmap_info)
     * - V7/V8: No transformation needed (default implementation does nothing)
     * 
     * @param modelData Model data to transform (modified in-place)
     */
    virtual void PreProcessModel(ModelDataBase& modelData) const {
        // Default: no preprocessing needed
        (void)modelData;
    }

private:
    int _taskBufferCount = DXRT_TASK_MAX_LOAD_VALUE;
};

/**
 * @brief Factory class for creating version-specific model parsers
 * 
 * This factory automatically detects the DXNN file version and creates
 * the appropriate parser instance.
 */
class DXRT_API ModelParserFactory {
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