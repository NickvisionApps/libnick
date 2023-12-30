#include "configurationbase.h"
#include <fstream>
#include <stdexcept>
#include "userdirectories.h"

namespace Nickvision::Aura
{
	ConfigurationBase::ConfigurationBase(const std::string& key) noexcept
		: m_key{ key },
		m_path{ UserDirectories::getApplicationConfig() / (key + ".json") }
	{
		if (std::filesystem::exists(m_path))
		{
			std::ifstream in{ m_path };
			in >> m_json;
		}
	}

	const std::string& ConfigurationBase::getKey() const noexcept
	{
		return m_key;
	}

	Events::Event<Events::EventArgs>& ConfigurationBase::saved() noexcept
	{
		return m_saved;
	}

	bool ConfigurationBase::save() noexcept
	{
		if (m_key.empty())
		{
			return false;
		}
		std::ofstream out{ m_path };
		out << m_json;
		m_saved.invoke({});
		return true;
	}
}