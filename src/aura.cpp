#include "aura.h"
#include <cstdlib>
#include <stdexcept>
#include "localization/gettext.h"
#include "helpers/stringhelpers.h"
#ifdef __linux__
#include <stdlib.h>
#endif

namespace Nickvision::Aura
{
	std::unique_ptr<Aura> Aura::m_instance = nullptr;

	Aura::Aura(const std::string& id, const std::string& name, const std::string& englishShortName)
	{
		m_appInfo.setId(id);
		m_appInfo.setName(name);
		m_appInfo.setEnglishShortName(englishShortName);
		std::string domainName{ StringHelpers::toLower(StringHelpers::replace(m_appInfo.getEnglishShortName(), " ", "")) };
		if (!Localization::Gettext::init(domainName))
		{
			throw std::runtime_error("Unable to initialize gettext.");
		}
	}

	AppInfo& Aura::getAppInfo()
	{
		return m_appInfo;
	}

	Aura& Aura::init(const std::string& id, const std::string& name, const std::string& englishShortName)
	{
		if (!m_instance)
		{
			m_instance = std::unique_ptr<Aura>(new Aura(id, name, englishShortName));
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

	std::string Aura::getEnvVar(const std::string& key)
	{
		char* var = std::getenv(key.c_str());
		if (var)
		{
			return { var };
		}
		return "";
	}

	bool Aura::setEnvVar(const std::string& key, const std::string& value)
	{
#ifdef _WIN32
		std::string var{ key + "=" + value };
		return _putenv(var.c_str()) == 0;
#elif defined(__linux__)
		return setenv(key.c_str(), value.c_str(), true) == 0;
#endif
	}
}