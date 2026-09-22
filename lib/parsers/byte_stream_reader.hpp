/*
 * Copyright (C) 2026- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#ifndef DXRT_BYTE_STREAM_READER_H
#define DXRT_BYTE_STREAM_READER_H
#include <cstdint>
#include <array>
#include <cstring>
#include <stdexcept>
#include <type_traits>
#include <vector>

/*
 */
template <typename T>
class ByteStreamArray {
    static_assert(std::is_arithmetic<T>::value && sizeof(T) == 1, "ByteStreamArray only supports 1-byte primitive types (e.g., uint8_t, char).");
public :
    ByteStreamArray(const ByteStreamArray&) = default;
    ByteStreamArray(T const* const data, size_t const size) : data_(data), size_(size) {
        if (data_ == nullptr && size_ > 0) {
            throw std::invalid_argument("ByteStreamArray cannot be initialized with a null pointer and non-zero size.");
        }
    }
    ~ByteStreamArray() = default;

    /**
     * @brief Calculate the CRC32 checksum of the byte array.
     * @return The CRC32 checksum value.
     */
    uint32_t crc32() const {
        uint32_t crc_val = 0xFFFFFFFF; 
        
        // cast T to uint8_t for CRC calculation
        const uint8_t* raw_bytes = reinterpret_cast<const uint8_t*>(data_);
        for (size_t i = 0; i < size_; ++i) {
            crc_val ^= raw_bytes[i];
            for (int32_t j = 0; j < 8; ++j) {
                crc_val = (crc_val >> 1) ^ ((crc_val & 1) ? CRC32_POLY : 0);
            }
        }
        return ~crc_val;
    }    
    /*
        * @brief Get the size of the array.
        * @return The number of elements in the array.
    */
    size_t size() const { return size_; }
    /*
        * @brief Get a pointer to the underlying data.
        * @return A pointer to the first element of the array.
    */
    T const* data() const { return data_; }
    
    /*
        * @brief Access an element by index.
        * @param index The index of the element to access.
        * @return A reference to the element at the specified index.
    */
    T const& operator[](size_t const index) const { 
        return data_[index]; 
    }

    /*
        * @brief Access an element by index with bounds checking.
        * @param index The index of the element to access.
        * @return A reference to the element at the specified index.
        * @throws std::out_of_range if the index is out of bounds.
    */
    T const& at(size_t const index) const {
        if (index >= size_) {
            throw std::out_of_range("ByteStreamArray index out of range.");
        }
        return data_[index];
    }

    T const* begin() const { return data_; }
    T const* end() const { return data_ + size_; }
    
private:
    static constexpr uint32_t CRC32_POLY = 0xEDB88320;
    T const* const data_;
    size_t const size_;    

public :
    ByteStreamArray() = delete;
    ByteStreamArray& operator=(const ByteStreamArray&) = delete;
};

class ByteStreamReader {
public :
    ByteStreamReader(uint8_t const* const byte_stream, size_t const stream_len)
        : byte_stream_(byte_stream), stream_len_(stream_len) {
        const uint16_t endianness_magic_number = 0x0100;
        is_big_endian_ = (*reinterpret_cast<const uint8_t*>(&endianness_magic_number) == 0x01);
    }
    ~ByteStreamReader() = default;

    size_t length() const { return stream_len_; }
    uint8_t const* data() const { return byte_stream_; }

    /*
        * Read a primitive type from the byte stream at the specified offset.
        * @tparam T The primitive type to read (e.g., int, float, double).
        * @param offset The offset in the byte stream to read from.
        * @return The value of type T read from the byte stream.
        * @throws std::out_of_range if the offset is out of bounds or if the stream is empty.
    */
    template <typename T>
    T read(size_t offset) const {
        static_assert(std::is_arithmetic<T>::value, "Only primitive types allowed.");
        if (validation(offset, sizeof(T)) == false) {
            throw std::out_of_range("Cannot read from an empty stream buffer or offset is out of bounds.");
        }
        T result;
        std::memcpy(&result, byte_stream_ + offset, sizeof(T));
        if (is_big_endian_) {
            auto_endian_swap(result);
        }

        return result;
    }

    /*
        * Read an array of primitive types from the byte stream at the specified offset.
        * @tparam T The primitive type to read (e.g., int, float, double).
        * @param offset The offset in the byte stream to read from.
        * @param count The number of elements to read.
        * @return A std::vector<T> containing the values read from the byte stream.
        * @throws std::out_of_range if the offset is out of bounds or if the stream is empty.   
    */
    template <typename T>
    std::vector<T> read(size_t offset, size_t count) const {
        static_assert(std::is_arithmetic<T>::value, "Only primitive types allowed.");
        if (validation(offset, sizeof(T) * count) == false) {
            throw std::out_of_range("Cannot read from an empty stream buffer or offset is out of bounds.");
        }
        std::vector<T> result(count);
        std::memcpy(result.data(), byte_stream_ + offset, sizeof(T) * count);
        if (is_big_endian_) {
            for (auto& item : result) {
                auto_endian_swap(item);
            }
        }
        return result;
    }
    /*
    * Read an array of 1-byte primitive types WITHOUT memory copy (Zero-copy).
    * @tparam T The 1-byte primitive type to view (e.g., uint8_t, char).
    * @param offset The offset in the byte stream to read from.
    * @param count The number of elements to view.
    * @return A ByteStreamArray<T> acting as a zero-copy view of the stream.
    */
    template <typename T>
    ByteStreamArray<T> readstream(size_t const offset, size_t const count) const {
        static_assert(std::is_arithmetic<T>::value && sizeof(T) == 1, "raw_read is only supported for 1-byte types to bypass endian swap.");
                      
        if (validation(offset, sizeof(T) * count) == false) {
            throw std::out_of_range("Cannot read past the end of the byte stream.");
        }

        const T* view_ptr = reinterpret_cast<const T*>(byte_stream_ + offset);
        return ByteStreamArray<T>(view_ptr, count);
    }

private :
    // ==========================================
    // Endian Swap Utilities
    // ==========================================
    static constexpr size_t  SWAP_SKIP_BYTE = 0x1; // type T size of 1byte, no need to swap

    template <typename T>
    static T swap_bytes_core(const T& val) {
        T swapped;
        const uint8_t* src = reinterpret_cast<const uint8_t*>(&val);
        uint8_t* dst = reinterpret_cast<uint8_t*>(&swapped);
        for (size_t i = 0; i < sizeof(T); ++i) {
            dst[i] = src[sizeof(T) - 1 - i];
        }
        return swapped;
    }

    // bigger than 2byte, do real swap
    template <typename T>
    static typename std::enable_if<std::is_arithmetic<T>::value && (sizeof(T) > SWAP_SKIP_BYTE), void>::type
    auto_endian_swap(T& val) {
        val = swap_bytes_core(val);
    }

    // 1byte swap - SKIP!
    template <typename T>
    static typename std::enable_if<std::is_arithmetic<T>::value && (sizeof(T) == SWAP_SKIP_BYTE), void>::type
    auto_endian_swap(T& /*val*/) {}

    // array swap
    template <typename T, std::size_t N>
    static void auto_endian_swap(std::array<T, N>& arr) {
        for (auto& item : arr) {
            auto_endian_swap(item);
        }
    }

    bool validation(size_t const offset, size_t const read_size) const {
        if (byte_stream_ == nullptr || stream_len_ == 0) {
            std::cerr << "Cannot read from an empty stream buffer." << std::endl;
            return false;
        }
        if (offset + read_size > stream_len_) {
            std::cerr << "Cannot read past the end of the byte stream." << std::endl;
            return false;
        }
        return true;
    }

    bool is_big_endian_{true};
    uint8_t const* const byte_stream_ {nullptr};
    size_t const stream_len_{0UL}; 

public:    
    ByteStreamReader() = delete;
    ByteStreamReader(const ByteStreamReader&) = delete;
    ByteStreamReader& operator=(const ByteStreamReader&) = delete;
};

#endif // DXRT_BYTE_STREAM_READER_H