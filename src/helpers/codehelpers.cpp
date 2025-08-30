#include "helpers/codehelpers.h"
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#else
#include <errno.h>
#include <string.h>
#endif

namespace Nickvision::Helpers
{
    size_t CodeHelpers::combineHash(size_t a, size_t b) noexcept
    {
        if constexpr(sizeof(size_t) >= 8)
        {
            a ^= b + 0x517cc1b727220a95 + (a << 6) + (a >> 2);
        }
        else
        {
            a ^= b + 0x9e3779b9 + (a << 6) + (a >> 2);
        }
        return a;
    }

    std::vector<std::string> CodeHelpers::convertUrlMapToVector(const std::unordered_map<std::string, std::string>& urls) noexcept
    {
        std::vector<std::string> vec;
        for (const std::pair<const std::string, std::string>& pair : urls)
        {
            vec.push_back(pair.first + " " + pair.second);
        }
        return vec;
    }

    std::string CodeHelpers::getLastSystemError() noexcept
    {
#ifdef _WIN32
        DWORD errorMessageID{ GetLastError() };
        if(errorMessageID == 0) 
        {
            return std::string();
        }
        LPSTR messageBuffer{ nullptr };
        size_t size{ FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, nullptr) };
        std::string message(messageBuffer, size);
        LocalFree(messageBuffer);
        return message;
#else
        return std::string(strerror(errno));
#endif
    }

    std::vector<std::byte> CodeHelpers::readFileBytes(const std::filesystem::path& path) noexcept
    {
        if(!std::filesystem::exists(path))
        {
            return {};
        }
        std::ifstream file{ path, std::ios::binary };
        if(file.good())
        {
            std::vector<std::byte> bytes(std::filesystem::file_size(path));
            file.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
            return bytes;
        }
        return {};
    }

    bool CodeHelpers::writeFileBytes(const std::filesystem::path& path, const std::vector<std::byte>& bytes, bool overwrite) noexcept
    {
        if(std::filesystem::exists(path) && !overwrite)
        {
            return false;
        }
        std::ofstream file{ path, std::ios::binary | std::ios::trunc };
        if(file.good())
        {
            file.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
            return true;
        }
        return false;
    }
}
