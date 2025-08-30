#include "app/jsonfilebase.h"
#include <fstream>
#include <stdexcept>
#include "filesystem/userdirectories.h"
#include "system/environment.h"

using namespace Nickvision::Filesystem;
using namespace Nickvision::System;

namespace Nickvision::App
{
    JsonFileBase::JsonFileBase(const std::string& key, const std::string& appName, bool isPortable)
        : m_key{ key }
    {
        if (m_key.empty())
        {
            throw std::invalid_argument("Key must not be empty.");
        }
        if(appName.empty())
        {
            throw std::invalid_argument("Application name must not be empty.");
        }
        m_path = (isPortable ? Environment::getExecutableDirectory() : UserDirectories::get(ApplicationUserDirectory::Config, appName)) / (m_key + ".json");
        if (std::filesystem::exists(m_path))
        {
            try
            {
                std::ifstream in{ m_path };
                boost::json::stream_parser parser;
                std::string line;
                while(std::getline(in, line))
                {
                    parser.write(line);
                }
                parser.finish();
                boost::json::value value = parser.release();
                if(value.is_object())
                {
                    m_json = value.as_object();
                }
            }
            catch(...) { }
        }
    }

    const std::string& JsonFileBase::getKey() const
    {
        return m_key;
    }

    Events::Event<Events::EventArgs>& JsonFileBase::saved()
    {
        return m_saved;
    }

    bool JsonFileBase::save()
    {
        std::ofstream out{ m_path };
        out << m_json << std::endl;
        m_saved({});
        return true;
    }

    boost::json::value JsonFileBase::toJson() const noexcept
    {
        return m_json;
    }
}
