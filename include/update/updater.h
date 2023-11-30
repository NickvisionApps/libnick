#ifndef UPDATER_H
#define UPDATER_H

#include <optional>
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
		 */
		Updater();
		/**
		 * @brief Gets the latest stable version from the GitHub repo.
		 * @return The current stable version, if available
		 */
		std::optional<Version> getCurrentStableVersion();
		/**
		 * @brief Gets the latest preview version from the GitHub repo.
		 * @return The current preview version, if available
		 */
		std::optional<Version> getCurrentPreviewVersion();
		/**
		 * @brief Downloads and installs an application update for Windows. getCurrentStableVersion or getCurrentPreviewVersion should be called first before running this method. This method will force quit the current running application to install the update.
		 * @param versionType The type of version update to install
		 * @return True if successful, else false
		 */
		bool windowsUpdate(VersionType versionType);

	private:
		std::string m_repoOwner;
		std::string m_repoName;
		int m_latestStableReleaseId;
		int m_latestPreviewReleaseId;
		std::optional<Version> getCurrentVersion(VersionType versionType);
	};
}

#endif //UPDATER_H