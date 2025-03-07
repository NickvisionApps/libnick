#include "system/hardwareinfo.h"
#include <fstream>
#include <string>
#include <thread>
#include "helpers/sizehelpers.h"
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <sys/sysinfo.h>
#elif defined(__APPLE__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

using namespace Nickvision::Helpers;

namespace Nickvision::System
{
    unsigned int HardwareInfo::getNumberOfProcessors()
    {
        return std::thread::hardware_concurrency();
    }

    unsigned long long HardwareInfo::getTotalRamSize()
    {
#ifdef _WIN32
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        if(GlobalMemoryStatusEx(&status))
        {
            return status.ullTotalPhys;
        }
#elif defined(__linux__)
        struct sysinfo info;
        if(sysinfo(&info) == 0)
        {
            return info.totalram;
        }
#elif defined(__APPLE__)
        int mib[2];
        unsigned long long ram;
        size_t len{ sizeof(ram) };
        mib[0] = CTL_HW;
        mib[1] = HW_MEMSIZE;
        if(sysctl(mib, 2, &ram, &len, nullptr, 0) == 0)
        {
            return ram;
        }
#endif
        return 0;
    }

    unsigned long long HardwareInfo::getFreeRamSize()
    {
#ifdef _WIN32
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        if(GlobalMemoryStatusEx(&status))
        {
            return status.ullAvailPhys;
        }
#elif defined(__linux__)
        struct sysinfo info;
        if(sysinfo(&info) == 0)
        {
            return info.freeram;
        }
#elif defined(__APPLE__)
        int mib[2];
        unsigned long long ram;
        size_t len{ sizeof(ram) };
        mib[0] = CTL_HW;
        mib[1] = HW_USERMEM;
        if(sysctl(mib, 2, &ram, &len, nullptr, 0) == 0)
        {
            return ram;
        }
#endif
        return 0;
    }
}
