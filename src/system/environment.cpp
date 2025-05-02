#include "system/environment.h"
#include <cstdlib>
#include <locale>
#include <sstream>
#include <unordered_map>
#include "helpers/stringhelpers.h"
#include "system/process.h"
#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <sys/syslimits.h>
#endif

using namespace Nickvision::App;
using namespace Nickvision::Helpers;

namespace Nickvision::System
{
    DeploymentMode Environment::getDeploymentMode()
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

    const std::filesystem::path& Environment::getExecutableDirectory()
    {
        static std::filesystem::path executableDirectory;
        if (!executableDirectory.empty())
        {
            return executableDirectory;
        }
#ifdef _WIN32
        wchar_t pth[MAX_PATH];
        DWORD len{ GetModuleFileNameW(nullptr, pth, sizeof(pth)) };
        if(len > 0)
        {
            executableDirectory = std::filesystem::path(std::wstring(pth, len)).parent_path();
        }
#elif defined(__linux__)
        executableDirectory = std::filesystem::canonical("/proc/self/exe").parent_path();
#elif defined(__APPLE__)
        char path[PATH_MAX+1];
        uint32_t size{ sizeof(path) };
        if(_NSGetExecutablePath(path, &size) == 0)
        {
            executableDirectory = std::filesystem::canonical(path).parent_path();
        }
#endif
        return executableDirectory;
    }

    const std::filesystem::path& Environment::getExecutablePath()
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

    std::string Environment::getLocaleName()
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

    bool Environment::hasVariable(const std::string& key)
    {
        return std::getenv(key.c_str());
    }

    std::string Environment::getVariable(const std::string& key)
    {
        char* var{ std::getenv(key.c_str()) };
        if (var)
        {
            return { var };
        }
        return "";
    }

    bool Environment::setVariable(const std::string& key, const std::string& value)
    {
#ifdef _WIN32
        return _putenv_s(key.c_str(), value.c_str()) == 0;
#else
        return setenv(key.c_str(), value.c_str(), true) == 0;
#endif
    }

    bool Environment::clearVariable(const std::string& key)
    {
        return setVariable(key, "");
    }
    
    bool Environment::testVariable(const std::string& key)
    {
        std::string value{ getVariable(key) };
        if(value.empty())
        {
            return false;
        }
        value = StringHelpers::lower(value);
        return value == "true" || value == "t" || value == "yes" || value == "y" || value == "1";
    }

    std::vector<std::filesystem::path> Environment::getPath()
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

    std::string Environment::exec(const std::string& command)
    {
        if(command.empty())
        {
            return "";
        }
        std::vector<std::string> args{ StringHelpers::splitArgs(command) };
#ifdef _WIN32
        args.insert(args.begin(), "/c");
        Process process{ findDependency("cmd.exe"), args };
#else
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

    const std::filesystem::path& Environment::findDependency(std::string dependency)
    {
        static std::unordered_map<std::string, std::filesystem::path> dependencies;
#ifdef _WIN32
        if(!std::filesystem::path(dependency).has_extension())
        {
            dependency += ".exe";
        }
#endif
        //Dependency already found once before
        if(dependencies.contains(dependency))
        {
            const std::filesystem::path& location{ dependencies[dependency] };
            //Return if path is still valid
            if(std::filesystem::exists(location))
            {
                return location;
            }
        }
        //Search for dependency
        dependencies[dependency] = std::filesystem::path();
        //Search in current executable's directory
        std::filesystem::path path{ getExecutableDirectory() / dependency };
        if(std::filesystem::exists(path))
        {
            dependencies[dependency] = path;
        }
        //Search in system PATH
        else
        {
            for(const std::filesystem::path& dir : getPath())
            {
                path = { dir / dependency };
                if(std::filesystem::exists(path) && dir.string().find("AppData\\Local\\Microsoft\\WindowsApps") == std::string::npos)
                {
                    dependencies[dependency] = path;
                    break;
                }
            }
        }
        //Return newly cached dependency path
        return dependencies[dependency];
    }

    std::string Environment::getDebugInformation(const AppInfo& appInfo, const std::string& extraInformation)
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
        builder << appInfo.getVersion() << std::endl << std::endl;
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
