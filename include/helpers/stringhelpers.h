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
 * Functions for working with strings.
 */

#ifndef STRINGHELPERS_H
#define STRINGHELPERS_H

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

namespace Nickvision::Helpers::StringHelpers
{
    template<typename T>
    concept StringImplicitlyConstructible = std::is_constructible_v<T, std::string> && std::is_convertible_v<std::string, T>;

    /**
     * @brief Converts a base64 encoded string into a list of bytes.
     * @param base64 The base64 encoded string
     * @return The bytes list from the base64 encoded string, empty list if error
     */
    std::vector<std::byte> decode(const std::string& base64);
    /**
     * @brief Converts a list of bytes into a base64 encoded string.
     * @param bytes The list of bytes
     * @return The base64 encoded string of the bytes list
     */
    std::string encode(const std::vector<std::byte>& bytes);
    /**
     * @brief Gets whether or not the provided string is a valid url
     * @param s The string to check
     * @return True if the string is a valid url, else false
     */
    bool isValidUrl(const std::string& s);
    /**
     * @brief Concatenates the elements of a string list using the specified separator between each element.
     * @param values The list of strings to join
     * @param separator The string to use as a separator between each element
     * @param separateLast Whether or not to include the separator for the last joined element
     * @return A single string that consists of all elements of the string list separated by the delimiter
     */
    std::string join(const std::vector<std::string>& values, const std::string& separator, bool separateLast = true);
    /**
     * @brief Gets a fully lowercase string from the provided string.
     * @param s The string to get lowercase
     * @return The new lowercase string
     */
    std::string lower(std::string s);
    /**
     * @brief Generates a new guid (uuid v4) value.
     * @return The guid value
     */
    std::string newGuid();
    /**
     * @brief Normalizes a string for use in a filename.
     * @param s The string to normalize
     * @param windowsOnly Whether or not to normalize strictly for Windows
     * @return The new normalized string
     */
    std::string normalizeForFilename(const std::string& s, bool windowsOnly = false);
    /**
     * @brief Replaces a substring within a string with a new string.
     * @param s The string to work on
     * @param toReplace The substring to be replaced
     * @param replace The new string to replace with
     * @return The new replaced string
     */
    std::string replace(std::string s, const std::string& toReplace, const std::string& replace);
    /**
     * @brief Replaces a character within a string with a new character.
     * @param s The string to work on
     * @param toReplace The character to be replaced
     * @param replace The new character to replace with
     * @return The new replaced string
     */
    std::string replace(std::string s, char toReplace, char replace);
    /**
     * @brief Splits a string based on a delimiter.
     * @tparam T The type of the resulting splits (must be a type that can be implicitly converted to string)
     * @param s The string to split
     * @param delimiter The delimiter to split the string on
     * @return The splits of the string
     */
    template<StringImplicitlyConstructible T = std::string>
    std::vector<T> split(std::string s, const std::string& delimiter)
    {
        if(s.empty())
        {
            return { "" };
        }
        std::vector<T> splits;
        if(delimiter.empty())
        {
            splits.push_back(s);
            return splits;
        }
        size_t pos{ 0 };
        while ((pos = s.find(delimiter)) != std::string::npos)
        {
            std::string split{ s.substr(0, pos) };
            splits.push_back(split);
            s.erase(0, pos + 1);
        }
        if (!s.empty())
        {
            splits.push_back(s);
        }
        return splits;
    }
    /**
     * @brief Splits a string based on argument delimiters.
     * @param s The string to split
     * @return The splits of the argument string
     */
    std::vector<std::string> splitArgs(std::string s);
    /**
     * @brief Converts the wstring to a string.
     * @param s The wstring to convert
     * @return The string version of the wstring
     */
    std::string str(const std::wstring& s);
    /**
     * @brief Converts a string to an unsigned int
     * @param s The string to convert
     * @param idx The address of a size_t to store the number of characters processed
     * @param base The number base
     * @return The unsigned int version of the string
     * @return UINT_MAX if the converted string is too long
     * @return 0 if error
     */
    unsigned int stoui(const std::string& s, size_t* idx = nullptr, int base = 10);
    /**
     * @brief Trims whitespace form the beginning and end of a string.
     * @param s The string to trim (unmodified)
     * @return The new trimmed string
     */
    std::string trim(const std::string& s);
    /**
     * @brief Trims the delimiter character form the beginning and end of a string.
     * @param s The string to trim (unmodified)
     * @param delimiter The character to trim
     * @return The new trimmed string
     */
    std::string trim(const std::string& s, char delimiter);
    /**
     * @brief Gets a fully uppercase string from the provided string.
     * @param s The string to get uppercase
     * @return The new uppercase string
     */
    std::string upper(std::string s);
    /**
     * @brief Converts the string to a wstring.
     * @param s The string to convert
     * @return The wstring version of the string
     */
    std::wstring wstr(const std::string& s);
}

#endif // STRINGHELPERS_H
