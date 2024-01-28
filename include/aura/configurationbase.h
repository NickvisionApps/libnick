#ifndef CONFIGURATIONBASE_H
#define CONFIGURATIONBASE_H

#include <filesystem>
#include <string>
#include <json/json.h>
#include "events/event.h"

namespace Nickvision::Aura
{
	/**
	 * A base class for configuration files.
	 */
	class ConfigurationBase
	{
	public:
		/**
		 * @brief Constructs a ConfigurationBase, loading the file from disk.
		 * @param key The key of the config file
		 * @throw std::invalid_argument Thrown if key is empty
		 */
		ConfigurationBase(const std::string& key);
		/**
		 * @brief Destructs a ConfigurationBase. 
		 */
		virtual ~ConfigurationBase() = default;
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

#endif //CONFIGURATIONBASE_H