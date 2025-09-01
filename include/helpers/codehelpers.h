/**
 * @file
 * @author Nicholas Logozzo <nlogozzo225@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Functions that make working with C++ easier.
 */

#ifndef CODEHELPERS_H
#define CODEHELPERS_H

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

#define DEFINE_ENUM_FLAGS(ENUM_TYPE) \
inline constexpr ENUM_TYPE operator~(ENUM_TYPE a) noexcept { \
    using underlying = std::underlying_type_t<ENUM_TYPE>; \
    return static_cast<ENUM_TYPE>(~static_cast<underlying>(a)); \
} \
inline constexpr ENUM_TYPE operator|(ENUM_TYPE lhs, ENUM_TYPE rhs) noexcept { \
    using underlying = std::underlying_type_t<ENUM_TYPE>; \
    return static_cast<ENUM_TYPE>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs)); \
} \
inline constexpr ENUM_TYPE operator&(ENUM_TYPE lhs, ENUM_TYPE rhs) noexcept { \
    using underlying = std::underlying_type_t<ENUM_TYPE>; \
    return static_cast<ENUM_TYPE>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs)); \
} \
inline constexpr ENUM_TYPE operator^(ENUM_TYPE lhs, ENUM_TYPE rhs) noexcept { \
    using underlying = std::underlying_type_t<ENUM_TYPE>; \
    return static_cast<ENUM_TYPE>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs)); \
} \
inline ENUM_TYPE& operator|=(ENUM_TYPE& lhs, ENUM_TYPE rhs) noexcept { \
    return lhs = (lhs | rhs); \
} \
inline ENUM_TYPE& operator&=(ENUM_TYPE& lhs, ENUM_TYPE rhs) noexcept { \
    return lhs = (lhs & rhs); \
} \
inline ENUM_TYPE& operator^=(ENUM_TYPE& lhs, ENUM_TYPE rhs) noexcept { \
    return lhs = (lhs ^ rhs); \
}

namespace Nickvision::Helpers::CodeHelpers
{
    /**
     * @brief Combines two hash values together.
     * @param a The first hash value
     * @param b The second hash value
     * @return The combined hash value
     */
    size_t combineHash(size_t a, size_t b) noexcept;
    /**
     * @brief Converts a map of URLs to a vector
     * @param urls The map of URLs
     * @return The vector of URLs
     */
    std::vector<std::string> convertUrlMapToVector(const std::unordered_map<std::string, std::string>& urls) noexcept;
    /**
     * @brief Get the last system api call error message.
     * @return The last system error message
     */
    std::string getLastSystemError() noexcept;
    /**
     * @brief Reads a file as a vector of bytes.
     * @param path The path to the file
     * @return The bytes from the file
     */
    std::vector<std::byte> readFileBytes(const std::filesystem::path& path) noexcept;
    /**
     * @brief Unmoves a value.
     * @param t An rvalue of T
     * @return The lvalue of t
     */
    template<typename T>
    const T& unmove(T&& t) noexcept
    {
        return t;
    }
    /**
     * @brief Writes a vector of bytes to a file.
     * @param path The path to the file
     * @param bytes The bytes to write
     * @param overwrite Whether or not to overwrite the file if it already exists
     * @return True if successful, false otherwise. False is returned if overwrite is false and the file exists.
     */
    bool writeFileBytes(const std::filesystem::path& path, const std::vector<std::byte>& bytes, bool overwrite = true) noexcept;
}

#endif //CODEHELPERS_H
