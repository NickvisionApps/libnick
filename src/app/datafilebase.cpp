#include "app/datafilebase.h"
#include <fstream>
#include <stdexcept>
#include "filesystem/userdirectories.h"

using namespace Nickvision::Filesystem;

namespace Nickvision::App
{
    DataFileBase::DataFileBase(const std::string& key, const std::string& appName)
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
        m_path = UserDirectories::get(ApplicationUserDirectory::Config, appName) / (m_key + ".json");
        if (std::filesystem::exists(m_path))
        {
            std::ifstream in{ m_path };
            boost::json::stream_parser parser;
            std::string line;
            while(std::getline(in, line))
            {
                parser.write_some(line);
            }
            parser.finish();
            boost::json::value value{ parser.release() };
            if(value.is_object())
            {
                m_json = value.as_object();
            }
        }
    }

    const std::string& DataFileBase::getKey() const
    {
        return m_key;
    }

    Events::Event<Events::EventArgs>& DataFileBase::saved()
    {
        return m_saved;
    }

    bool DataFileBase::save()
    {
        std::ofstream out{ m_path };
        out << m_json << std::endl;
        m_saved({});
        return true;
    }
}