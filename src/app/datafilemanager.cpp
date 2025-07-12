#include "app/datafilemanager.h"

namespace Nickvision::App
{
    DataFileManager::DataFileManager(const std::string& appName, bool isPortable)
        : m_appName{ appName },
        m_isPortable{ isPortable }
    {
        
    }
}
