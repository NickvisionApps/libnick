#include "aura.h"
#include <stdexcept>

namespace Nickvision::Aura
{
	std::optional<Aura> Aura::m_instance = std::nullopt;

	Aura::Aura(const std::string& id, const std::string& name)
	{
		m_appInfo.setId(id);
		m_appInfo.setName(name);
	}

	Aura::~Aura()
	{
		for (const std::pair<std::string, ConfigurationBase*>& pair : m_configFiles)
		{
			delete pair.second;
		}
	}

	AppInfo& Aura::getAppInfo()
	{
		return m_appInfo;
	}

	Aura& Aura::init(const std::string& id, const std::string& name)
	{
		if (!m_instance)
		{
			m_instance = { id, name };
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
}