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
         * @brief Gets the latest version of the provided type from the GitHub repo.
         * @brief This method looks for tags in the format major.minor.build-dev for preview versions and major.minor.build for stable versions.
         * @param versionType The type of the version to get
         * @return The current version of the provided type if available, else empty Version
         */
        Version fetchCurrentVersion(VersionType versionType);
#ifdef _WIN32
        /**
         * @brief Downloads and installs an application update for Windows. 
         * @brief fetchCurrentVersion should be called first before running this method.
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
        mutable std::mutex m_mutex;
        std::string m_repoOwner;
        std::string m_repoName;
        int m_latestStableReleaseId;
        int m_latestPreviewReleaseId;
    };
}

#endif //UPDATER_H