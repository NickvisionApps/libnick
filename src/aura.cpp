#include "aura.h"
#include <cstdlib>
#include <stdexcept>

namespace Nickvision::Aura
{
	std::unique_ptr<Aura> Aura::m_instance = nullptr;

	Aura::Aura(const std::string& id, const std::string& name)
	{
		m_appInfo.setId(id);
		m_appInfo.setName(name);
	}

	AppInfo& Aura::getAppInfo()
	{
		return m_appInfo;
	}

	Aura& Aura::init(const std::string& id, const std::string& name)
	{
		if (!m_instance)
		{
			m_instance = std::unique_ptr<Aura>(new Aura(id, name));
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
}