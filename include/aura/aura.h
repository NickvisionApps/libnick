#if (defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS))
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef AURA_H
#define AURA_H

#include <filesystem>
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
        Aura(const Aura&) = delete;
        void operator=(const Aura&) = delete;
		/**
		 * @brief Destructs an Aura.
		 */
		~Aura();
        /**
		 * @brief Initializes Aura.
		 * @brief This also calls curl_global_init().
		 * @brief This also calls Localization::Gettext::init().
		 * @param id The application id
		 * @param name The application name
		 * @param englishShortName The application short name in English
		 * @throw std::runtime_error Thrown if libcurl fails to initialize
		 * @throw std::runtime_error Thrown if the gettext system fails to initialize
         * @throw std::runtime_error Thrown if unable to get the executable directory path
         * @return True if initialized, else false
		 */
        bool init(const std::string& id, const std::string& name, const std::string& englishShortName);
		/**
		 * @brief Gets the AppInfo object for the application.
		 */
		AppInfo& getAppInfo();
        /**
		 * @brief Gets the path of the executable's directory.
		 * @return The executable's directory path
		 */
		const std::filesystem::path& getExecutableDirectory() const;
		/**
		 * @brief Gets a system environment variable.
		 * @param key The environment variable to get
		 * @return The environment variable if found, else empty string
		 */
		std::string getEnvVar(const std::string& key);
		/**
		 * @brief Sets a system environment variable.
		 * @param key The environment variable to set
		 * @param value The value for the environment variable
		 * @return True if set, else false
		 */
		bool setEnvVar(const std::string& key, const std::string& value);
		/**
		 * @brief Finds the path of a given dependency.
		 * @param dependency The name of the dependency to find
		 * @return The path of the dependency if found, else empty path
		 */
		const std::filesystem::path& findDependency(std::string dependency);
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
		 */
		Aura();
        bool m_initialized;
		AppInfo m_appInfo;
        std::filesystem::path m_executableDirectory;
        std::map<std::string, std::filesystem::path> m_dependencies;
        std::map<std::string, std::unique_ptr<ConfigurationBase>> m_configFiles;

	public:
		/**
		 * @brief Gets the active aura instance.
		 * @return The active aura instance
		 */
		static Aura& getActive();
	};
}

#endif //AURA_H