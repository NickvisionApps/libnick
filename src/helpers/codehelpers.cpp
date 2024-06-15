#include "helpers/codehelpers.h"
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <errno.h>
#include <string.h>
#endif

namespace Nickvision::Helpers
{
    std::string CodeHelpers::getLastSystemError()
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
#elif defined(__linux__)
        return std::string(strerror(errno));
#endif
    }

    std::vector<std::byte> CodeHelpers::readFileBytes(const std::filesystem::path& path)
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

    bool CodeHelpers::writeFileBytes(const std::filesystem::path& path, const std::vector<std::byte>& bytes, bool overwrite)
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