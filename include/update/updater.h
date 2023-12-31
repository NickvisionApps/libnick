#ifndef UPDATER_H
#define UPDATER_H

#include <mutex>
#include <string>
#include "../version.h"

namespace Nickvision::Aura::Update
{
	/**
	 * @brief An object to check for application updates through GitHub.
	 */
	class Updater
	{
	public:
		/**
		 * @brief Constructs an Updater.
		 * @param githubRepoUrl The url of the GitHub repo to fetch updates for. Must be a valid github url. If the url is invalid, Updater will attempt to get a repo url from Aura
		 * @throw std::invalid_argument Thrown if no valid GitHub url can be determined
		 */
		Updater(std::string githubRepoUrl = "");
		/**
		 * @brief Gets the latest stable version from the GitHub repo.
		 * @return The current stable version if available, else empty Version
		 */
		Version fetchCurrentStableVersion() noexcept;
		/**
		 * @brief Gets the latest preview version from the GitHub repo.
		 * @return The current preview version if available, else empty Version
		 */
		Version fetchCurrentPreviewVersion() noexcept;
#ifdef _WIN32
		/**
		 * @brief Downloads and installs an application update for Windows. getCurrentStableVersion or getCurrentPreviewVersion should be called first before running this method. This method will force quit the current running application to install the update.
		 * @param versionType The type of version update to install
		 * @return True if successful, else false
		 */
		bool windowsUpdate(VersionType versionType) noexcept;
#endif

	private:
		/**
		 * @brief Gets the latest version of the provided type from the GitHub repo
		 * @param versionType The type of the version to get
		 * @return The current version of the proivded type if available, else empty Version
		 */
		Version fetchCurrentVersion(VersionType versionType) noexcept;
		mutable std::mutex m_mutex;
		std::string m_repoOwner;
		std::string m_repoName;
		int m_latestStableReleaseId;
		int m_latestPreviewReleaseId;
	};
}

#endif //UPDATER_H