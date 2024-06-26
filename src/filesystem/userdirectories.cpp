#include "filesystem/userdirectories.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include "helpers/stringhelpers.h"
#include "system/environment.h"
#ifdef _WIN32
#include <shlobj_core.h>
#else
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#endif 

using namespace Nickvision::Helpers;
using namespace Nickvision::System;

namespace Nickvision::Filesystem
{
#ifdef _WIN32
    static std::filesystem::path getKnownDir(REFKNOWNFOLDERID rfid)
    {
        std::filesystem::path result;
        wchar_t* p{ nullptr };
        if (SHGetKnownFolderPath(rfid, 0, nullptr, &p) == S_OK)
        {
            result = p;
        }
        CoTaskMemFree(static_cast<void*>(p));
        return result;
    }
#elif defined(__linux__)
    static std::filesystem::path getXDGDir(const std::string& key)
    {
        std::string var{ Environment::getVariable(key) };
        if (!var.empty())
        {
            return var;
        }
        std::filesystem::path dirsPath{ UserDirectories::get(UserDirectory::Config) / "user-dirs.dirs" };
        if (!std::filesystem::exists(dirsPath))
        {
            return {};
        }
        std::ifstream dirs{ dirsPath };
        std::string line;
        while (std::getline(dirs, line))
        {
            if (line[0] == '#' || line.empty())
            {
                continue;
            }
            std::vector<std::string> pair{ StringHelpers::split(line, "=") };
            if (pair[0] == key)
            {
                if (pair[1].find("$HOME") != std::string::npos)
                {
                    pair[1].replace(pair[1].find("$HOME"), 5, UserDirectories::getHome().string());
                }
                return StringHelpers::trim(pair[1], '"');
            }
        }
        return { };
    }
#endif

    std::filesystem::path UserDirectories::get(UserDirectory directory, const std::string& appName)
    {
        std::filesystem::path result;
        switch(directory)
        {
        case UserDirectory::Home:
        {
#ifdef _WIN32
            result = getKnownDir(FOLDERID_Profile);
#else
            std::filesystem::path var{ Environment::getVariable("HOME") };
            result = !var.empty() ? var : getpwuid(getuid())->pw_dir;
#endif
            break;
        }
        case UserDirectory::Config:
        {
#ifdef _WIN32
            result = getKnownDir(FOLDERID_RoamingAppData);
#elif defined(__APPLE__)
            result = (get(UserDirectory::Home) / "Library/Application Support");
#else
            std::filesystem::path var{ Environment::getVariable("XDG_CONFIG_HOME") };
            result = !var.empty() ? var : (get(UserDirectory::Home) / ".config");
#endif
            break;
        }
        case UserDirectory::ApplicationConfig:
        {
            if(appName.empty())
            {
                return result;
            }
            result = get(UserDirectory::Config) / appName;
            break;
        }
        case UserDirectory::Cache:
        {
#ifdef _WIN32
            result = getKnownDir(FOLDERID_LocalAppData) / "Temp";
#elif defined(__APPLE__)
            result = (get(UserDirectory::Home) / "Library/Caches");
#else
            std::filesystem::path var{ Environment::getVariable("XDG_CACHE_HOME") };
            result = !var.empty() ? var : (get(UserDirectory::Home) / ".cache");
#endif
            break;
        }
        case UserDirectory::ApplicationCache:
        {
            if(appName.empty())
            {
                return result;
            }
            result = get(UserDirectory::Cache) / appName;
            break;
        }
        case UserDirectory::LocalData:
        {
#ifdef _WIN32
            result = getKnownDir(FOLDERID_LocalAppData);
#elif defined(__APPLE__)
            result = get(UserDirectory::Config);
#else
            std::filesystem::path var{ Environment::getVariable("XDG_DATA_HOME") };
            result = !var.empty() ? var : (get(UserDirectory::Home) / ".local/share");
#endif
            break;
        }
        case UserDirectory::ApplicationLocalData:
        {
            if(appName.empty())
            {
                return result;
            }
            result = get(UserDirectory::LocalData) / appName;
            break;
        }
        case UserDirectory::Desktop:
        {
#ifdef _WIN32
            result = getKnownDir(FOLDERID_Desktop);
#elif defined(__APPLE__)
            result = (get(UserDirectory::Home) / "Desktop");
#else
            result = getXDGDir("XDG_DESKTOP_DIR");
            result = result.empty() ? (get(UserDirectory::Home) / "Desktop") : result;
#endif
            break;
        }
        case UserDirectory::Documents:
        {
#ifdef _WIN32
            result = getKnownDir(FOLDERID_Documents);
#elif defined(__APPLE__)
            result = (get(UserDirectory::Home) / "Documents");
#else
            result = getXDGDir("XDG_DOCUMENTS_DIR");
            result = result.empty() ? (get(UserDirectory::Home) / "Documents") : result;
#endif
            break;
        }
        case UserDirectory::Downloads:
        {
#ifdef _WIN32
            result = getKnownDir(FOLDERID_Downloads);
#elif defined(__APPLE__)
            result = (get(UserDirectory::Home) / "Downloads");
#else
            result = getXDGDir("XDG_DOWNLOAD_DIR");
            result = result.empty() ? (get(UserDirectory::Home) / "Downloads") : result;
#endif
            break;
        }
        case UserDirectory::Music:
        {
#ifdef _WIN32
            result = getKnownDir(FOLDERID_Music);
#elif defined(__APPLE__)
            result = (get(UserDirectory::Home) / "Music");
#else
            result = getXDGDir("XDG_MUSIC_DIR");
            result = result.empty() ? (get(UserDirectory::Home) / "Music") : result;
#endif
            break;
        }
        case UserDirectory::Pictures:
        {
#ifdef _WIN32
            result = getKnownDir(FOLDERID_Pictures);
#elif defined(__APPLE__)
            result = (get(UserDirectory::Home) / "Pictures");
#else
            result = getXDGDir("XDG_PICTURES_DIR");
            result = result.empty() ? (get(UserDirectory::Home) / "Pictures") : result;
#endif
            break;
        }
        case UserDirectory::Templates:
        {
#ifdef _WIN32
            result = getKnownDir(FOLDERID_Templates);
#elif defined(__APPLE__)
            result = (get(UserDirectory::Home) / "Templates");
#else
            result = getXDGDir("XDG_TEMPLATES_DIR");
            result = result.empty() ? (get(UserDirectory::Home) / "Templates") : result;
#endif
            break;
        }
        case UserDirectory::Videos:
        {
#ifdef _WIN32
            result = getKnownDir(FOLDERID_Videos);
#elif defined(__APPLE__)
            result = (get(UserDirectory::Home) / "Videos");
#else
            result = getXDGDir("XDG_VIDEOS_DIR");
            result = result.empty() ? (get(UserDirectory::Home) / "Videos") : result;
#endif
            break;
        }
        default:
            return result;
        }
        std::filesystem::create_directories(result);
        return result;
    }
}