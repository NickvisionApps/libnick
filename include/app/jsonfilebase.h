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
 * A base class for json data files.
 */

#ifndef JSONFILEBASE_H
#define JSONFILEBASE_H

#include <filesystem>
#include <string>
#include <boost/json.hpp>
#include "events/event.h"
#include "helpers/ijsonserializable.h"

namespace Nickvision::App
{
    /**
     * @brief A base class for json data files.
     */
    class JsonFileBase : public Helpers::IJsonSerializable
    {
    public:
        /**
         * @brief Constructs a JsonFileBase, loading the file from disk.
         * @param key The key of the config file
         * @param appName The name of the application the data file belongs to
         * @param isPortable Whether or not the config file is portable (will save to executable directory instead of config directory)
         * @throw std::invalid_argument Thrown if key is empty
         * @throw std::invalid_argument Thrown if appName is empty
         */
        JsonFileBase(const std::string& key, const std::string& appName, bool isPortable);
        /**
         * @brief Destructs a JsonFileBase.
         */
        virtual ~JsonFileBase() = default;
        /**
         * Gets the key of the config file.
         * @return The key of the config file
         */
        const std::string& getKey() const;
        /**
         * @brief Gets the Saved event.
         * @return The Saved event
         */
        Events::Event<Events::EventArgs>& saved();
        /**
         * @brief Saves the config file to disk. 
         * @return True if saved to disk, else false
         */
        bool save();
        /**
         * @brief Serializes the object to Json.
         * @return The Json representation of the object
         */
        boost::json::value toJson() const noexcept override;

    protected:
        mutable boost::json::object m_json;

    private:
        std::string m_key;
        std::filesystem::path m_path;
        Events::Event<Events::EventArgs> m_saved;
    };
}

#endif //JSONFILEBASE_H
