#include "filesystem/systemdirectories.h"
#include "helpers/stringhelpers.h"
#include "system/environment.h"

using namespace Nickvision::System;

namespace Nickvision::Filesystem
{
    static std::vector<std::filesystem::path> getFromVar(const std::string& var)
    {
        std::string env{ Environment::getVariable(var) };
        if (!env.empty())
        {
#ifdef _WIN32
            return StringHelpers::split<std::filesystem::path>(env, ";");
#elif defined(__linux__)
            return StringHelpers::split<std::filesystem::path>(env, ":");
#endif
        }
        return {};
    }

    std::vector<std::filesystem::path> SystemDirectories::getPath()
    {
        return getFromVar("PATH");
    }

    std::vector<std::filesystem::path> SystemDirectories::getConfig()
    {
        return getFromVar("XDG_CONFIG_DIRS");
    }

    std::vector<std::filesystem::path> SystemDirectories::getData()
    {
        return getFromVar("XDG_DATA_DIRS");
    }
}