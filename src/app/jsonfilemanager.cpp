#include "app/jsonfilemanager.h"

namespace Nickvision::App
{
    JsonFileManager::JsonFileManager(const std::string& appName, bool isPortable)
        : m_appName{ appName },
        m_isPortable{ isPortable }
    {
        
    }

    JsonFileManager::JsonFileManager(JsonFileManager&& other) noexcept
        : m_appName{ std::move(other.m_appName) },
        m_isPortable{ other.m_isPortable },
        m_files{ std::move(other.m_files) }
    {
        other.m_files.clear();
    }

    JsonFileManager& JsonFileManager::operator=(JsonFileManager&& other) noexcept
    {
        if (this != &other)
        {
            m_appName = std::move(other.m_appName);
            m_isPortable = other.m_isPortable;
            m_files = std::move(other.m_files);
            other.m_files.clear();
        }
        return *this;
    }
}
