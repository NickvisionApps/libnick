#ifndef DATAFILEBASE_H
#define DATAFILEBASE_H

#include <filesystem>
#include <string>
#include <json/json.h>
#include "events/event.h"

namespace Nickvision::App
{
    /**
     * A base class for json data files.
     */
    class DataFileBase
    {
    public:
        /**
         * @brief Constructs a DataFileBase, loading the file from disk.
         * @param key The key of the config file
         * @param appName The name of the application the data file belongs to
         * @throw std::invalid_argument Thrown if key is empty
         */
        DataFileBase(const std::string& key, const std::string& appName);
        /**
         * @brief Destructs a DataFileBase. 
         */
        virtual ~DataFileBase() = default;
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

    protected:
        Json::Value m_json;

    private:
        std::string m_key;
        std::filesystem::path m_path;
        Events::Event<Events::EventArgs> m_saved;
    };
}

#endif //DATAFILEBASE_H