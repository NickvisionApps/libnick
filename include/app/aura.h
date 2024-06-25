#ifndef AURA_H
#define AURA_H

#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include "appinfo.h"
#include "configurationbase.h"
#include "interprocesscommunicator.h"
#include "logging/logger.h"
#include "notifications/notifyicon.h"

namespace Nickvision::App
{
    template<typename T>
    concept DerivedConfigurationBase = std::is_base_of_v<ConfigurationBase, T>;

    /**
     * @brief An application base.
     */
    class Aura
    {
    public:
        // Delete copy and move constructors and assignment operators
        Aura(const Aura&) = delete;
        Aura(Aura&&) = delete;
        void operator=(const Aura&) = delete;
        void operator=(Aura&&) = delete;
        /**
         * @brief Initializes Aura.
         * @brief This also calls curl_global_init().
         * @brief This also calls Localization::Gettext::init().
         * @param id The application id
         * @param name The application name
         * @param englishShortName The application short name in English
         * @param logLevel The application log level
         * @throw std::runtime_error Thrown if unable to get the executable directory path
         * @throw std::runtime_error Thrown if the gettext system fails to initialize
         * @return True if initialized, else false
         */
        bool init(const std::string& id, const std::string& name, const std::string& englishShortName, Logging::LogLevel logLevel = Logging::LogLevel::Info);
        /**
         * @brief Gets whether or not the object is valid.
         * @return True if valid, else false
         */
        bool isValid() const;
        /**
         * @brief Gets the AppInfo object for the application.
         */
        AppInfo& getAppInfo();
        /**
         * @brief Gets the application's InterProcessCommunicator object.
         * @return The InterProcessCommunicator
         */
        InterProcessCommunicator& getIPC();
        /**
         * @brief Gets the application's Logger object.
         * @return The Logger
         */
        const Logging::Logger& getLogger() const;
        /**
         * @brief Gets the url for a documentation page.
         * @brief This will be a yelp url for Linux and a website url for Windows and Linux snaps.
         * @brief HtmlDocsStore should be set for Aura::getActive()::getAppInfo()
         * @param pageName The name of the page to get the url for
         * @return The url for the documentation page
         */
        std::string getHelpUrl(const std::string& pageName);
        /**
         * @brief Checks if the object is valid.
         * @return True if valid, else false
         */
        operator bool() const;
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
#ifdef _WIN32
        /**
         * @brief Gets the application's NotifyIcon.
         * @brief This will create the NotifyIcon on the first call. The icon will be hidden by default.
         * @param hwnd The window handle. Must be valid for the first call.
         * @return The NotifyIcon
         */
        Notifications::NotifyIcon& getNotifyIcon(HWND hwnd);
#endif

    private:
        /**
         * @brief Constructs an Aura.
         */
        Aura();
        bool m_initialized;;
        AppInfo m_appInfo;
        std::unique_ptr<InterProcessCommunicator> m_ipc;
        std::unordered_map<std::string, std::unique_ptr<ConfigurationBase>> m_configFiles;
        std::unique_ptr<Logging::Logger> m_logger;
#ifdef _WIN32
        std::unique_ptr<Notifications::NotifyIcon> m_notifyIcon;
#endif

    public:
        /**
         * @brief Gets the active aura instance.
         * @return The active aura instance
         */
        static Aura& getActive();
    };
}

#endif //AURA_H
