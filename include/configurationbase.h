#ifndef CONFIGURATIONBASE_H
#define CONFIGURATIONBASE_H

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
		 */
		ConfigurationBase(const std::string& key);
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
		 */
		void save();

	protected:
		Json::Value m_json;
		std::string m_key;
		Events::Event<Events::EventArgs> m_saved;
	};
}

#endif //CONFIGURATIONBASE_H