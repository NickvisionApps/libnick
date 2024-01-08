#if (defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS))
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef AURA_H
#define AURA_H

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include "appinfo.h"
#include "configurationbase.h"

namespace Nickvision::Aura
{
	template<typename T>
	concept DerivedConfigurationBase = std::is_base_of_v<ConfigurationBase, T>;

	/**
	 * @brief An application base.
	 */
	class Aura
	{
	public:
		/**
		 * @brief Destructs an Aura.
		 */
		~Aura() noexcept;
		/**
		 * @brief Gets the AppInfo object for the application.
		 */
		AppInfo& getAppInfo() noexcept;
		/**
		 * @brief Gets a config object.
		 * @tparam T Derived type of ConfigurationBase
		 * @param key The key of the config file
		 * @throw std::invalid_argument Thrown if key is empty
		 * @return The config object
		 */
		template<DerivedConfigurationBase T>
		T& getConfig(const std::string& key)
		{
			if (key.empty())
			{
				throw std::invalid_argument("Key must not be empty.");
			}
			if (!m_configFiles.contains(key))
			{
				m_configFiles[key] = std::make_unique<T>(key);
			}
			return *static_cast<T*>(m_configFiles[key].get());
		}

	private:
		/**
		 * @brief Constructs an Aura.
		 * @brief This also calls curl_global_init().
		 * @brief This also calls Localization::Gettext::init().
		 * @param id The application id
		 * @param name The application name
		 * @param englishShortName The application short name in English
		 * @throw std::runtime_error Thrown if libcurl fails to initialize
		 * @throw std::runtime_error Thrown if the gettext system fails to initialize
		 */
		Aura(const std::string& id, const std::string& name, const std::string& englishShortName);
		AppInfo m_appInfo;
		std::map<std::string, std::unique_ptr<ConfigurationBase>> m_configFiles;

	public:
		/**
		 * @brief Initializes Aura.
		 * @brief This also calls curl_global_init().
		 * @brief This also calls Localization::Gettext::init().
		 * @param id The application id
		 * @param name The application name
		 * @param englishShortName The application short name in English
		 * @throw std::runtime_error Thrown if creation of Aura object fails
		 * @return The active aura instance
		 */
		static Aura& init(const std::string& id, const std::string& name, const std::string& englishShortName);
		/**
		 * @brief Gets the active aura instance. Aura::init() must have been called first.
		 * @throw std::logic_error Thrown if Aura::init() was not yet called
		 * @return The active aura instance
		 */
		static Aura& getActive();
		/**
		 * @brief Gets a system environment variable.
		 * @param key The environment variable to get
		 * @return The environment variable if found, else empty string
		 */
		static std::string getEnvVar(const std::string& key) noexcept;
		/**
		 * @brief Sets a system environment variable.
		 * @param key The environment variable to set
		 * @param value The value for the environment variable
		 * @return True if set, else false
		 */
		static bool setEnvVar(const std::string& key, const std::string& value) noexcept;
		/**
		 * @brief Finds the path of a given dependency.
		 * @param dependency The name of the dependency to find
		 * @return The path of the dependency if found, else empty path
		 */
		static const std::filesystem::path& findDependency(std::string dependency) noexcept;

	private:
		static std::unique_ptr<Aura> m_instance;
	};
}

#endif //AURA_H