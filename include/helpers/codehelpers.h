#ifndef CODEHELPERS_H
#define CODEHELPERS_H

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

namespace Nickvision::Helpers::CodeHelpers
{
    /**
     * @brief Get the last system api call error message.
     * @return The last system error message
     */
    std::string getLastSystemError();
    /**
     * @brief Reads a file as a vector of bytes.
     * @param path The path to the file
     * @return The bytes from the file
     */
    std::vector<std::byte> readFileBytes(const std::filesystem::path& path);
    /**
     * @brief Unmoves a value.
     * @param t An rvalue of T
     * @return The lvalue of t
     */
    template<typename T>
    const T& unmove(T&& t)
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
    bool writeFileBytes(const std::filesystem::path& path, const std::vector<std::byte>& bytes, bool overwrite = true);
}

#endif //CODEHELPERS_H
