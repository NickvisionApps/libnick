#include "configurationbase.h"
#include <fstream>
#include <stdexcept>
#include "userdirectories.h"

namespace Nickvision::Aura
{
	ConfigurationBase::ConfigurationBase(const std::string& key)
		: m_key{ key }
	{
		std::filesystem::path path{ UserDirectories::getApplicationConfig() / (key + ".json") };
		if (std::filesystem::exists(path))
		{
			std::ifstream in{ path };
			in >> m_json;
		}
	}

	const std::string& ConfigurationBase::getKey() const
	{
		return m_key;
	}

	Events::Event<Events::EventArgs>& ConfigurationBase::saved()
	{
		return m_saved;
	}

	void ConfigurationBase::save()
	{
		if (m_key.empty())
		{
			throw std::invalid_argument("Key must not be empty.");
		}
		std::ofstream out{ UserDirectories::getApplicationConfig() / (m_key + ".json") };
		out << m_json;
		m_saved.invoke({});
	}
}