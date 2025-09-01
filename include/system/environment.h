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
 * Functions for working with the apps's shell and environment.
 */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#if (defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS))
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <filesystem>
#include <future>
#include <string>
#include <vector>
#include "app/appinfo.h"
#include "dependencysearchoption.h"
#include "deploymentmode.h"
#include "operatingsystem.h"

/**
 * @brief Functions for working with the apps's shell and environment.
 */
namespace Nickvision::System::Environment
{
    /**
     * @brief Gets the current operating system.
     * @return The current operating system
     */
    constexpr OperatingSystem getOperatingSystem() noexcept
    {
#ifdef _WIN32
        return OperatingSystem::Windows;
#elif defined(__linux__)
        return OperatingSystem::Linux;
#elif defined(__APPLE__)
        return OperatingSystem::MacOS;
#else
        return OperatingSystem::Other;
#endif
    }
    /**
     * @brief Gets the current deployment mode.
     * @return The current deployment mode
     */
    DeploymentMode getDeploymentMode() noexcept;
    /**
     * @brief Gets the path of the executable's directory.
     * @return The executable's directory path
     */
    const std::filesystem::path& getExecutableDirectory() noexcept;
    /**
     * @brief Gets the path of the executable file.
     * @return The executable file's path
     */
    const std::filesystem::path& getExecutablePath() noexcept;
    /**
     * @brief Gets the name of the current locale.
     * @return The locale name
     */
    std::string getLocaleName() noexcept;
    /**
     * @brief Checks if an environment variable exists.
     * @param key The environment variable to check
     * @return True if the environment variable exists, else false
     */
    bool hasVariable(const std::string& key) noexcept;
    /**
     * @brief Gets the value of an environment variable.
     * @param key The environment variable to get
     * @return The environment variable value if found, else empty string
     */
    std::string getVariable(const std::string& key) noexcept;
    /**
     * @brief Sets the value of an environment variable.
     * @param key The environment variable to set
     * @param value The value for the environment variable
     * @return True if set, else false
     */
    bool setVariable(const std::string& key, const std::string& value) noexcept;
    /**
     * @brief Clears an environment variable.
     * @param key The environment variable to delete
     * @return True if deleted, else false
     */
    bool clearVariable(const std::string& key) noexcept;
    /**
     * @brief Tests an environment variable value to see if it is true.
     * @brief A true value is one that is "true", "t", "yes", "y", or "1".
     * @param key The environment variable to test
     * @return True if the value is true, else false
     */
    bool testVariable(const std::string& key) noexcept;
    /**
     * @brief Gets a list of directories from the system PATH variable.
     * @return The list of directories from PATH
     */
    std::vector<std::filesystem::path> getPath() noexcept;
    /**
     * @brief Executes a command in the system shell.
     * @param cmd The command to execute
     * @return The output of the command
     */
    std::string exec(const std::string& cmd) noexcept;
    /**
     * @brief Executes a command asynchronously in the system shell.
     * @param cmd The command to execute
     * @return The future containing the output of the command
     */
    std::future<std::string> execAsync(const std::string& cmd) noexcept;
    /**
     * @brief Finds the path of a given executable dependency in the system.
     * @param dependency The name of the executable dependency to find
     * @param search The DependencySearchOption to use in the dependency search
     * @return The path to the executable dependency if found, else empty path
     */
    const std::filesystem::path& findDependency(std::string dependency, DependencySearchOption search = DependencySearchOption::Global) noexcept;
    /**
     * @brief Gets a debug information string about the user's environment.
     * @brief appInfo The application's AppInfo
     * @brief extraInformation Extra information to append to the end of the debug information string
     * @return The debug information string
     */
    std::string getDebugInformation(const App::AppInfo& appInfo, const std::string& extraInformation = "") noexcept;
}

#endif //ENVIRONMENT_H
