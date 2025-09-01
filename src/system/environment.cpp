#include "system/environment.h"
#include <cstdlib>
#include <locale>
#include <sstream>
#include <unordered_map>
#include <utility>
#include "filesystem/userdirectories.h"
#include "helpers/pairhash.h"
#include "helpers/stringhelpers.h"
#include "system/process.h"
#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <sys/syslimits.h>
#endif

using namespace Nickvision::App;
using namespace Nickvision::Filesystem;
using namespace Nickvision::Helpers;

namespace Nickvision::System
{
    DeploymentMode Environment::getDeploymentMode() noexcept
    {
        if(hasVariable("container"))
        {
            return DeploymentMode::Flatpak;
        }
        else if(hasVariable("SNAP"))
        {
            return DeploymentMode::Snap;
        }
        return DeploymentMode::Local;
    }

    const std::filesystem::path& Environment::getExecutableDirectory() noexcept
    {
        static std::filesystem::path executableDirectory{ getExecutablePath().parent_path() };
        return executableDirectory;
    }

    const std::filesystem::path& Environment::getExecutablePath() noexcept
    {
        static std::filesystem::path executablePath;
        if (!executablePath.empty())
        {
            return executablePath;
        }
#ifdef _WIN32
        wchar_t pth[MAX_PATH];
        DWORD len{ GetModuleFileNameW(nullptr, pth, sizeof(pth)) };
        if(len > 0)
        {
            executablePath = std::filesystem::path(std::wstring(pth, len));
        }
#elif defined(__linux__)
        executablePath = std::filesystem::canonical("/proc/self/exe");
#elif defined(__APPLE__)
        char path[PATH_MAX+1];
        uint32_t size{ sizeof(path) };
        if(_NSGetExecutablePath(path, &size) == 0)
        {
            executablePath = std::filesystem::canonical(path);
        }
#endif
        return executablePath;
    }

    std::string Environment::getLocaleName() noexcept
    {
#ifdef _WIN32
        LCID lcid{ GetThreadLocale() };
        wchar_t name[LOCALE_NAME_MAX_LENGTH];
        if(LCIDToLocaleName(lcid, name, LOCALE_NAME_MAX_LENGTH, 0) > 0)
        {
            return StringHelpers::str(name);
        }
#else
        try
        {
            return std::locale("").name();
        }
        catch(...) { }
#endif
        return "";
    }

    bool Environment::hasVariable(const std::string& key) noexcept
    {
        return std::getenv(key.c_str());
    }

    std::string Environment::getVariable(const std::string& key) noexcept
    {
        char* var{ std::getenv(key.c_str()) };
        if (var)
        {
            return { var };
        }
        return "";
    }

    bool Environment::setVariable(const std::string& key, const std::string& value) noexcept
    {
#ifdef _WIN32
        return _putenv_s(key.c_str(), value.c_str()) == 0;
#else
        return setenv(key.c_str(), value.c_str(), true) == 0;
#endif
    }

    bool Environment::clearVariable(const std::string& key) noexcept
    {
        return setVariable(key, "");
    }
    
    bool Environment::testVariable(const std::string& key) noexcept
    {
        std::string value{ getVariable(key) };
        if(value.empty())
        {
            return false;
        }
        value = StringHelpers::lower(value);
        return value == "true" || value == "t" || value == "yes" || value == "y" || value == "1";
    }

    std::vector<std::filesystem::path> Environment::getPath() noexcept
    {
        std::string env{ getVariable("PATH") };
        if (!env.empty())
        {
#ifdef _WIN32
            return StringHelpers::split<std::filesystem::path>(env, ";");
#else
            return StringHelpers::split<std::filesystem::path>(env, ":");
#endif
        }
        return {};
    }

    std::string Environment::exec(const std::string& command) noexcept
    {
        if(command.empty())
        {
            return "";
        }
#ifdef _WIN32
        Process process{ findDependency("cmd.exe"), { "/c", command } };
#else
        std::vector<std::string> args{ StringHelpers::splitArgs(command) };
        std::string cmd{ args[0] };
        args.erase(args.begin());
        Process process{ cmd, args };
#endif
        if(process.start())
        {
            process.waitForExit();
            return process.getOutput();
        }
        return "";
    }

    std::future<std::string> Environment::execAsync(const std::string& command) noexcept
    {
        return std::async(std::launch::async, [command]() { return exec(command); });
    }

    const std::filesystem::path& Environment::findDependency(std::string dependency, DependencySearchOption search) noexcept
    {
        static std::unordered_map<std::pair<std::string, DependencySearchOption>, std::filesystem::path, PairHash> dependencies;
#ifdef _WIN32
        if(!std::filesystem::path(dependency).has_extension())
        {
            dependency += ".exe";
        }
#endif
        std::pair<std::string, DependencySearchOption> pair{ std::make_pair(dependency, search) };
        //Dependency already found once before
        if(dependencies.contains(pair))
        {
            const std::filesystem::path& location{ dependencies[pair] };
            //Return if path is still valid
            if(std::filesystem::exists(location))
            {
                return location;
            }
        }
        //Search for dependency
        dependencies[pair] = std::filesystem::path();
        if(search == DependencySearchOption::Global) //Executable directory, than PATH
        {
            std::filesystem::path path{ getExecutableDirectory() / dependency };
            if(std::filesystem::exists(path))
            {
                dependencies[pair] = path;
            }
            else
            {
                for(const std::filesystem::path& dir : getPath())
                {
                    path = { dir / dependency };
                    if(std::filesystem::exists(path) && dir.string().find("AppData\\Local\\Microsoft\\WindowsApps") == std::string::npos)
                    {
                        dependencies[pair] = path;
                        break;
                    }
                }
            }
        }
        else if(search == DependencySearchOption::App) //Executable directory only
        {
            std::filesystem::path path{ getExecutableDirectory() / dependency };
            if(std::filesystem::exists(path))
            {
                dependencies[pair] = path;
            }
        }
        else if(search == DependencySearchOption::System) //PATH only
        {
            std::filesystem::path path;
            for(const std::filesystem::path& dir : getPath())
            {
                path = { dir / dependency };
                if(std::filesystem::exists(path) && dir.string().find("AppData\\Local\\Microsoft\\WindowsApps") == std::string::npos)
                {
                    dependencies[pair] = path;
                    break;
                }
            }
        }
        else if(search == DependencySearchOption::Local) //Local data folder only
        {
            std::filesystem::path path{ UserDirectories::get(UserDirectory::LocalData) / dependency };
            if(std::filesystem::exists(path))
            {
                dependencies[pair] = path;
            }
        }
        return dependencies[pair];
    }

    std::string Environment::getDebugInformation(const AppInfo& appInfo, const std::string& extraInformation) noexcept
    {
        std::stringstream builder;
        builder << appInfo.getId() << std::endl;
        switch(getOperatingSystem())
        {
        case OperatingSystem::Windows:
            builder << "Windows" << std::endl;
            break;
        case OperatingSystem::MacOS:
            builder << "macOS" << std::endl;
            break;
        case OperatingSystem::Linux:
            builder << "Linux" << std::endl;
            break;
        default:
            builder << "Unknown OS" << std::endl;
            break;
        }
        builder << appInfo.getVersion().str() << std::endl << std::endl;
        builder << "Deployment Mode: ";
        switch(getDeploymentMode())
        {
        case DeploymentMode::Local:
            builder << "Local" << std::endl;
            break;
        case DeploymentMode::Flatpak:
            builder << "Flatpak" << std::endl;
            break;
        case DeploymentMode::Snap:
            builder << "Snap" << std::endl;
            break;
        }
        builder << "Locale: " << getLocaleName() << std::endl;
        builder << "Running From: " << getExecutableDirectory() << std::endl;
        if(!extraInformation.empty())
        {
            builder << std::endl << extraInformation;
        }
        return builder.str();
    }
}
