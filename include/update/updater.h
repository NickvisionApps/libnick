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
 * An object to check for application updates through GitHub.
 */

#ifndef UPDATER_H
#define UPDATER_H

#include <filesystem>
#include <mutex>
#include <string>
#include <cpr/callback.h>
#include "version.h"

namespace Nickvision::Update
{
    /**
     * @brief An object to check for and download new releases through GitHub.
     */
    class Updater
    {
    public:
        /**
         * @brief Constructs an Updater.
         * @param githubRepoUrl The url of the GitHub repo to fetch updates for
         * @throw std::invalid_argument Thrown if the GitHub url is not of valid format
         */
        Updater(std::string githubRepoUrl);
        /**
         * @brief Constructs an Updater via copy.
         * @param u The object to copy 
         */
        Updater(const Updater& u) noexcept;
            /**
         * @brief Constructs an Updater via move.
         * @param u The object to move 
         */
        Updater(Updater&& u) noexcept;
        /**
         * @brief Gets the latest version of the provided type from the GitHub repo.
         * @brief This method looks for tags in the format major.minor.build-dev or major.minor.build.dev for preview versions and major.minor.build for stable versions.
         * @brief If the VersionType is Preview, the updater may return a stable version if it is greater than the latest preview.
         * @param versionType The type of the version to get
         * @return The current version of the provided type if available, else empty Version
         */
        Version fetchCurrentVersion(VersionType versionType) noexcept;
        /**
         * @brief Downloads an update for the application.
         * @brief fetchCurrentVersion should be called first before running this method.
         * @param versionType The type of version update to download
         * @param path The path to save the downloaded file
         * @param assetName The name of the asset to download
         * @param exactMatch If true, the asset name must match exactly, else it can be a substring
         * @param progress An optional cpr::ProgressCallback to track the downloading update
         * @return True if successful, else false
         */
        bool downloadUpdate(VersionType versionType, const std::filesystem::path& path, const std::string& assetName, bool exactMatch = true, const cpr::ProgressCallback& progress = {}) noexcept;
#ifdef _WIN32
        /**
         * @brief Downloads and installs an application update for Windows. 
         * @brief fetchCurrentVersion should be called first before running this method.
         * @brief This method will download the asset that contains "setup.exe" in its name and run it.
         * @param versionType The type of version update to install
         * @param progress An optional cpr::ProgressCallback to track the downloading update
         * @return True if successful, else false
         */
        bool windowsUpdate(VersionType versionType, const cpr::ProgressCallback& progress = {}) noexcept;
#endif
        /**
         * @brief Copies an Updater.
         * @param u The Updater to copy
         * @return this
         */
        Updater& operator=(const Updater& u) noexcept;
        /**
         * @brief Moves an Updater.
         * @param u The Updater to move
         * @return this
         */
        Updater& operator=(Updater&& u) noexcept;

    private:
        mutable std::mutex m_mutex;
        std::string m_repoOwner;
        std::string m_repoName;
        int m_latestStableReleaseId;
        int m_latestPreviewReleaseId;
    };
}

#endif //UPDATER_H
