#include "app/configurationbase.h"
#include <fstream>
#include <stdexcept>
#include "app/aura.h"
#include "filesystem/userdirectories.h"

using namespace Nickvision::Filesystem;

namespace Nickvision::App
{
    ConfigurationBase::ConfigurationBase(const std::string& key)
        : m_key{ key }
    {
        if (m_key.empty())
        {
            throw std::invalid_argument("Key must not be empty.");
        }
        m_path = UserDirectories::get(UserDirectory::ApplicationCache, Aura::getActive().getAppInfo().getName()) / (key + ".json");
        if (std::filesystem::exists(m_path))
        {
            std::ifstream in{ m_path };
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

    bool ConfigurationBase::save()
    {
        std::ofstream out{ m_path };
        out << m_json;
        m_saved({});
        return true;
    }
}