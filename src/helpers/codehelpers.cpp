#include "helpers/codehelpers.h"
#ifdef _WIN32
#include <windows.h>
#else
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
#else
        return std::string(strerror(errno));
#endif
    }
}