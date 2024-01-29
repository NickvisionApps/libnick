#ifndef UPDATER_H
#define UPDATER_H

#include <mutex>
#include <string>
#include "version.h"

namespace Nickvision::Update
{
    /**
     * @brief An object to check for application updates through GitHub.
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
        Updater(const Updater& u);
            /**
         * @brief Constructs an Updater via move.
         * @param u The object to move 
         */
        Updater(Updater&& u) noexcept;
        /**
         * @brief Gets the latest stable version from the GitHub repo.
         * @return The current stable version if available, else empty Version
         */
        Version fetchCurrentStableVersion();
        /**
         * @brief Gets the latest preview version from the GitHub repo.
         * @return The current preview version if available, else empty Version
         */
        Version fetchCurrentPreviewVersion();
#ifdef _WIN32
        /**
         * @brief Downloads and installs an application update for Windows. getCurrentStableVersion or getCurrentPreviewVersion should be called first before running this method. This method will force quit the current running application to install the update.
         * @param versionType The type of version update to install
         * @return True if successful, else false
         */
        bool windowsUpdate(VersionType versionType);
#endif
        /**
         * @brief Copies an Updater.
         * @param u The Updater to copy
         * @return this
         */
        Updater& operator=(const Updater& u);
        /**
         * @brief Moves an Updater.
         * @param u The Updater to move
         * @return this
         */
        Updater& operator=(Updater&& u) noexcept;

    private:
        /**
         * @brief Gets the latest version of the provided type from the GitHub repo
         * @param versionType The type of the version to get
         * @return The current version of the proivded type if available, else empty Version
         */
        Version fetchCurrentVersion(VersionType versionType);
        mutable std::mutex m_mutex;
        std::string m_repoOwner;
        std::string m_repoName;
        int m_latestStableReleaseId;
        int m_latestPreviewReleaseId;
    };
}

#endif //UPDATER_H