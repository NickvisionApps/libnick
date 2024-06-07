#include "system/environment.h"
#include <cstdlib>
#include "helpers/stringhelpers.h"
#include "system/process.h"
#ifdef _WIN32
#include <windows.h>
#endif

namespace Nickvision::System
{
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
#elif defined(__linux__)
        return setenv(key.c_str(), value.c_str(), true) == 0;
#endif
    }

    bool Environment::clearVariable(const std::string& key)
    {
        return setVariable(key, "");
    }

    std::string Environment::exec(const std::string& command)
    {
        if(command.empty())
        {
            return "";
        }
#ifdef _WIN32
        Process process{ "cmd.exe", { "/C", "\"" + command + "\"" } };
#elif defined(__linux__)
        std::vector<std::string> args{ StringHelpers::split(command, " ") };
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
}