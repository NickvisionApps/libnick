/**
 * @file
 * @author Nicholas Logozzo <nlogozzo225@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * A manager of json files for an application.
 */

#ifndef JSONFILEMANAGER_H
#define JSONFILEMANAGER_H

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include "jsonfilebase.h"

namespace Nickvision::App
{
    template<typename T>
    concept DerivedJsonFileBase = std::is_base_of_v<JsonFileBase, T>;

    /**
     * @brief A manager of json files for an application.
     */
    class JsonFileManager
    {
    public:
        /**
         * @brief Constructs a JsonFileManager.
         * @param appName The name of the application (used in determining the path to store json files on disk)
         * @param isPortable Whether or not the application is portable (the config files should live in the same exe directory)
         */
        JsonFileManager(const std::string& appName, bool isPortable);
        JsonFileManager(const JsonFileManager&) = delete;
        /**
         * @brief Constructs a JsonFileManager via move.
         * @param other The other JsonFileManager to move
         */
        JsonFileManager(JsonFileManager&& other) noexcept;
        /**
         * @brief Gets a json object.
         * @tparam T Derived type of JsonFileBase
         * @param key The key of the json file
         * @throw std::invalid_argument Thrown if key is empty
         * @return The json object
         */
        template<DerivedJsonFileBase T>
        T& get(const std::string& key)
        {
            if (key.empty())
            {
                throw std::invalid_argument("Key must not be empty.");
            }
            if (!m_files.contains(key))
            {
                m_files[key] = std::make_unique<T>(key, m_appName, m_isPortable);
            }
            return *static_cast<T*>(m_files[key].get());
        }
        JsonFileManager& operator=(const JsonFileManager&) = delete;
        /**
         * @brief Assigns a JsonFileManager via move.
         * @param other The other JsonFileManager to move
         * @return A reference to this JsonFileManager
         */
        JsonFileManager& operator=(JsonFileManager&& other) noexcept;

    private:
        std::string m_appName;
        bool m_isPortable;
        std::unordered_map<std::string, std::unique_ptr<JsonFileBase>> m_files;
    };
}

#endif //DATAFILEMANAGER_H
