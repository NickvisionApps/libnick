#include "aura/aura.h"
#include <cstdlib>
#include <curl/curl.h>
#include "filesystem/systemdirectories.h"
#include "localization/gettext.h"
#include "helpers/stringhelpers.h"
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <stdlib.h>
#endif

using namespace Nickvision::Filesystem;

namespace Nickvision::Aura
{
	Aura::Aura()
        : m_initialized{ false }
	{

	}

	Aura::~Aura()
	{
        if(m_initialized)
        {
            curl_global_cleanup();
        }
	}

	bool Aura::init(const std::string& id, const std::string& name, const std::string& englishShortName)
	{
        if(!m_initialized)
        {
            //Setup AppInfo
            m_appInfo.setId(id);
            m_appInfo.setName(name);
            m_appInfo.setEnglishShortName(englishShortName);
            //Setup curl
            if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0)
            {
                throw std::runtime_error("Unable to initialize curl.");
            }
            //Setup gettext
            std::string domainName{ StringHelpers::toLower(StringHelpers::replace(m_appInfo.getEnglishShortName(), " ", "")) };
            if (!Localization::Gettext::init(domainName))
            {
                throw std::runtime_error("Unable to initialize gettext.");
            }
            //Get executable's directory path
#ifdef _WIN32
            char pth[MAX_PATH];
            DWORD len{ GetModuleFileNameA(nullptr, pth, sizeof(pth)) };
            if(len > 0)
            {
                m_executableDirectory = std::filesystem::path(std::string(pth, len)).parent_path();
            }
            else
            {
                throw std::runtime_error("Unable to get executable directory.");
            }
#elif defined(__linux__)
		    m_executableDirectory = std::filesystem::canonical("/proc/self/exe").parent_path();
#endif
            m_initialized = true;
        }
        return m_initialized;
	}

	AppInfo& Aura::getAppInfo()
	{
		return m_appInfo;
	}

	const std::filesystem::path& Aura::getExecutableDirectory() const
	{
        return m_executableDirectory;
	}

	std::string Aura::getEnvVar(const std::string& key)
	{
		char* var{ std::getenv(key.c_str()) };
		if (var)
		{
			return { var };
		}
		return "";
	}

	bool Aura::setEnvVar(const std::string& key, const std::string& value)
	{
#ifdef _WIN32
		return _putenv_s(key.c_str(), value.c_str()) == 0;
#elif defined(__linux__)
		return setenv(key.c_str(), value.c_str(), true) == 0;
#endif
	}

	const std::filesystem::path& Aura::findDependency(std::string dependency)
	{
#ifdef _WIN32
		if (!std::filesystem::path(dependency).has_extension())
		{
			dependency += ".exe";
		}
#endif
		if (m_dependencies.contains(dependency))
		{
			const std::filesystem::path& location{ m_dependencies[dependency] };
			if (std::filesystem::exists(location))
			{
				return location;
			}
		}
		m_dependencies[dependency] = std::filesystem::path();
		std::filesystem::path path{ getExecutableDirectory() / dependency };
		if (std::filesystem::exists(path))
		{
			m_dependencies[dependency] = path;
		}
		else
		{
			for (const std::filesystem::path& dir : SystemDirectories::getPath())
			{
				path = { dir / dependency };
				if (std::filesystem::exists(path) && dir.string().find("AppData\\Local\\Microsoft\\WindowsApps") == std::string::npos)
				{
					m_dependencies[dependency] = path;
					break;
				}
			}
		}
		return m_dependencies[dependency];
	}

    Aura& Aura::getActive()
	{
		static Aura aura;
        return aura;
	}
}