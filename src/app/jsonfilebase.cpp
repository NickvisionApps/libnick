#include "app/jsonfilebase.h"
#include <fstream>
#include <stdexcept>

namespace Nickvision::App
{
    JsonFileBase::JsonFileBase(const std::filesystem::path& path)
        : m_path{ path }
    {
        if (m_path.empty())
        {
            throw std::invalid_argument("Path must not be empty.");
        }
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

    const std::filesystem::path& JsonFileBase::getPath() const noexcept
    {
        std::lock_guard lock{ m_mutex };
        return m_path;
    }

    Events::Event<Events::EventArgs>& JsonFileBase::saved() noexcept
    {
        std::lock_guard lock{ m_mutex };
        return m_saved;
    }

    bool JsonFileBase::save() noexcept
    {
        std::lock_guard lock{ m_mutex };
        if(m_path.has_parent_path())
        {
            std::filesystem::create_directories(m_path.parent_path());
        }
        std::ofstream out{ m_path };
        out << m_json << std::endl;
        m_saved({});
        return true;
    }

    boost::json::value JsonFileBase::toJson() const noexcept
    {
        std::lock_guard lock{ m_mutex };
        return m_json;
    }

    bool JsonFileBase::contains(const std::string& key) const noexcept
    {
        std::lock_guard lock{ m_mutex };
        return m_json.contains(key);
    }
}
