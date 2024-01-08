#include "aura/aura.h"
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <curl/curl.h>
#include "filesystem/systemdirectories.h"
#include "localization/gettext.h"
#include "helpers/stringhelpers.h"
#ifdef __linux__
#include <stdlib.h>
#endif

using namespace Nickvision::Filesystem;

namespace Nickvision::Aura
{
	std::unique_ptr<Aura> Aura::m_instance = nullptr;

	Aura::Aura(const std::string& id, const std::string& name, const std::string& englishShortName)
	{
		m_appInfo.setId(id);
		m_appInfo.setName(name);
		m_appInfo.setEnglishShortName(englishShortName);
		if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0)
		{
			throw std::runtime_error("Unable to initialize curl.");
		}
		std::string domainName{ StringHelpers::toLower(StringHelpers::replace(m_appInfo.getEnglishShortName(), " ", "")) };
		if (!Localization::Gettext::init(domainName))
		{
			throw std::runtime_error("Unable to initialize gettext.");
		}
	}

	Aura::~Aura()
	{
		curl_global_cleanup();
	}

	AppInfo& Aura::getAppInfo() noexcept
	{
		return m_appInfo;
	}

	Aura& Aura::init(const std::string& id, const std::string& name, const std::string& englishShortName)
	{
		if (!m_instance)
		{
			if (!(m_instance = std::unique_ptr<Aura>(new Aura(id, name, englishShortName))))
			{
				throw std::runtime_error("Unable to make Aura object.");
			}
		}
		return *m_instance;
	}

	Aura& Aura::getActive()
	{
		if (!m_instance)
		{
			throw std::logic_error("Aura::init() must be called first.");
		}
		return *m_instance;
	}

	std::string Aura::getEnvVar(const std::string& key) noexcept
	{
		char* var{ std::getenv(key.c_str()) };
		if (var)
		{
			return { var };
		}
		return "";
	}

	bool Aura::setEnvVar(const std::string& key, const std::string& value) noexcept
	{
#ifdef _WIN32
		return _putenv_s(key.c_str(), value.c_str()) == 0;
#elif defined(__linux__)
		return setenv(key.c_str(), value.c_str(), true) == 0;
#endif
	}

	const std::filesystem::path& Aura::findDependency(std::string dependency) noexcept
	{
		static std::map<std::string, std::filesystem::path> locations;
		static std::mutex mutex;
		std::lock_guard<std::mutex> lock{ mutex };
#ifdef _WIN32
		if (!std::filesystem::path(dependency).has_extension())
		{
			dependency += ".exe";
		}
#endif
		if (locations.contains(dependency))
		{
			const std::filesystem::path& location{ locations[dependency] };
			if (std::filesystem::exists(location))
			{
				return location;
			}
		}
		locations[dependency] = std::filesystem::path();
		std::filesystem::path path{ std::filesystem::current_path() / dependency };
		if (std::filesystem::exists(path))
		{
			locations[dependency] = path;
		}
		else
		{
			for (const std::filesystem::path& dir : SystemDirectories::getPath())
			{
				path = { dir / dependency };
				if (std::filesystem::exists(path) && dir.string().find("AppData\\Local\\Microsoft\\WindowsApps") == std::string::npos)
				{
					locations[dependency] = path;
					break;
				}
			}
		}
		return locations[dependency];
	}
}