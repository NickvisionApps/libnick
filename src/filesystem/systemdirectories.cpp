#include "filesystem/systemdirectories.h"
#include "app/aura.h"
#include "helpers/stringhelpers.h"

using namespace Nickvision::App;

namespace Nickvision::Filesystem
{
    static std::vector<std::filesystem::path> getFromVar(const std::string& var)
    {
        std::string env{ Aura::getActive().getEnvVar(var) };
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