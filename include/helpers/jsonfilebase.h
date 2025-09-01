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
 * A base class for thread-safe json data files.
 */

#ifndef JSONFILEBASE_H
#define JSONFILEBASE_H

#include <filesystem>
#include <mutex>
#include <string>
#include <boost/json.hpp>
#include "events/event.h"
#include "ijsonserializable.h"

namespace Nickvision::Helpers
{
    /**
     * @brief A base class for thread-safe json data files.
     */
    class JsonFileBase : public IJsonSerializable
    {
    public:
        /**
         * @brief Constructs a JsonFileBase, loading the file from disk.
         * @param path The path to the json file
         * @throw std::invalid_argument Thrown if the path is empty
         */
        JsonFileBase(const std::filesystem::path& path);
        /**
         * @brief Destructs a JsonFileBase.
         */
        virtual ~JsonFileBase() noexcept = default;
        /**
         * @brief Gets the path of the json file.
         * @return The path of the json file
         */
        const std::filesystem::path& getPath() const noexcept;
        /**
         * @brief Gets the Saved event.
         * @return The Saved event
         */
        Events::Event<Events::EventArgs>& saved() noexcept;
        /**
         * @brief Saves the config file to disk. 
         * @return True if saved to disk, else false
         */
        bool save() noexcept;
        /**
         * @brief Serializes the object to Json.
         * @return The Json representation of the object
         */
        boost::json::value toJson() const noexcept override;

    protected:
        /**
         * @brief Gets whether or not the json object contains a key.
         */
        bool contains(const std::string& key) const noexcept;
        /**
         * @brief Gets a value from the json object.
         * @tparam T The type of the value to get
         * @param key The key of the value to get
         * @param defaultValue The default value to return if the key is not found or if the value is not of the expected type
         * @return The value associated with the key
         */
        template<SupportedJsonValue T>
        T get(const std::string& key, const T& defaultValue) const noexcept
        {
            std::lock_guard lock{ m_mutex };
            if constexpr (std::is_same_v<T, int>)
            {
                if(!m_json.contains(key) || !m_json[key].is_int64())
                {
                    return defaultValue;
                }
                return static_cast<int>(m_json[key].as_int64());
            }
            else if constexpr (std::is_same_v<T, std::int64_t>)
            {
                if(!m_json.contains(key) || !m_json[key].is_int64())
                {
                    return defaultValue;
                }
                return m_json[key].as_int64();
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                if(!m_json.contains(key) || !m_json[key].is_double())
                {
                    return defaultValue;
                }
                return m_json[key].as_double();
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                if(!m_json.contains(key) || !m_json[key].is_bool())
                {
                    return defaultValue;
                }
                return m_json[key].as_bool();
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                if(!m_json.contains(key) || !m_json[key].is_string())
                {
                    return defaultValue;
                }
                return m_json[key].as_string();
            }
            else if constexpr (std::is_same_v<T, boost::json::array>)
            {
                if(!m_json.contains(key) || !m_json[key].is_array())
                {
                    return defaultValue;
                }
                return m_json[key].as_array();
            }
            else if constexpr (std::is_same_v<T, boost::json::object>)
            {
                if(!m_json.contains(key) || !m_json[key].is_object())
                {
                    return defaultValue;
                }
                return m_json[key].as_object();
            }
        }
        /**
         * @brief Sets a value in the json object.
         * @tparam T The type of the value to set
         * @param key The key of the value to set
         * @param value The value to set
         */
        template<SupportedJsonValue T>
        void set(const std::string& key, const T& value) noexcept
        {
            std::lock_guard lock{ m_mutex };
            m_json[key] = value;
        }

    private:
        mutable std::mutex m_mutex;
        std::filesystem::path m_path;
        mutable boost::json::object m_json;
        Events::Event<Events::EventArgs> m_saved;
    };
}

#endif //JSONFILEBASE_H
