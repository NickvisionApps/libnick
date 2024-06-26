#ifndef DATAFILEMANAGER_H
#define DATAFILEMANAGER_H

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include "datafilebase.h"

namespace Nickvision::App
{
    template<typename T>
    concept DerivedDataFileBase = std::is_base_of_v<DataFileBase, T>;

    /**
     * @brief A manager of data files for an application.
     */
    class DataFileManager
    {
    public:
        /**
         * @brief Constructs a DataFileManager.
         * @param appName The name of the application (used in determining the path to store data files on disk)
         */
        DataFileManager(const std::string& appName);
        // Delete copy and move constructors and assignment operators
        DataFileManager(const DataFileManager&) = delete;
        DataFileManager(DataFileManager&&) = delete;
        void operator=(const DataFileManager&) = delete;
        void operator=(DataFileManager&&) = delete;
        /**
         * @brief Gets a data object.
         * @tparam T Derived type of DataFileBase
         * @param key The key of the data file
         * @throw std::invalid_argument Thrown if key is empty
         * @return The data object
         */
        template<DerivedDataFileBase T>
        T& get(const std::string& key)
        {
            if (key.empty())
            {
                throw std::invalid_argument("Key must not be empty.");
            }
            if (!m_files.contains(key))
            {
                m_files[key] = std::make_unique<T>(key, m_appName);
            }
            return *static_cast<T*>(m_files[key].get());
        }

    private:
        std::string m_appName;
        std::unordered_map<std::string, std::unique_ptr<DataFileBase>> m_files;
    };
}

#endif //DATAFILEMANAGER_H