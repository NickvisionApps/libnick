#ifndef USERDIRECTORIES_H
#define USERDIRECTORIES_H

#include <filesystem>

namespace Nickvision::Aura::UserDirectories
{
	/*
	 * @brief Gets the user's home directory
	 * @return FOLDERID_Profile on Windows, $HOME on Linux
	 */
	std::filesystem::path getHome();
	/*
	 * @brief Gets the user's config directory
	 * @return FOLDERID_RoamingAppData on Windows, XDG_CONFIG_HOME on Linux
	 */
	std::filesystem::path getConfig();
	/*
	 * @brief Gets the applications's config directory
	 * @return getConfig() + Aura::getActive().getAppInfo().getName()
	 */
	std::filesystem::path getApplicationConfig();
	/*
	 * @brief Gets the user's cache directory
	 * @return FOLDERID_LocalAppData on Windows, XDG_CACHE_HOME on Linux
	 */
	std::filesystem::path getCache();
	/*
	 * @brief Gets the applications's cache directory
	 * @return getCache() + Aura::getActive().getAppInfo().getName()
	 */
	std::filesystem::path getApplicationCache();
	/*
	 * @brief Gets the user's local data directory
	 * @return FOLDERID_RoamingAppData on Windows, XDG_DATA_HOME on Linux
	 */
	std::filesystem::path getLocalData();
	/*
	 * @brief Gets the applications's local data directory
	 * @return getLocalData() + Aura::getActive().getAppInfo().getName()
	 */
	std::filesystem::path getApplicationLocalData();
	/*
	 * @brief Gets the user's runtime directory
	 * @return Empty path on Windows, XDG_RUNTIME_DIR on Linux
	 */
	std::filesystem::path getRuntime();
	/*
	 * @brief Gets the user's desktop directory
	 * @return FOLDERID_Desktop on Windows, XDG_DESKTOP_DIR on Linux
	 */
	std::filesystem::path getDesktop();
	/*
	 * @brief Gets the user's documents directory
	 * @return FOLDERID_Documents on Windows, XDG_DOCUMENTS_DIR on Linux
	 */
	std::filesystem::path getDocuments();
	/*
	 * @brief Gets the user's downloads directory
	 * @return FOLDERID_Downloads on Windows, XDG_DOWNLOAD_DIR on Linux
	 */
	std::filesystem::path getDownloads();
	/*
	 * @brief Gets the user's music directory
	 * @return FOLDERID_Music on Windows, XDG_MUSIC_DIR on Linux
	 */
	std::filesystem::path getMusic();
	/*
	 * @brief Gets the user's pictures directory
	 * @return FOLDERID_Pictures on Windows, XDG_PICTURES_DIR on Linux
	 */
	std::filesystem::path getPictures();
	/*
	 * @brief Gets the user's public share directory
	 * @return Empty path on Windows, XDG_PUBLICSHARE_DIR on Linux
	 */
	std::filesystem::path getPublicShare();
	/*
	 * @brief Gets the user's templates directory
	 * @return FOLDERID_Templates on Windows, XDG_TEMPLATES_DIR on Linux
	 */
	std::filesystem::path getTemplates();
	/*
	 * @brief Gets the user's videos directory
	 * @return FOLDERID_Videos on Windows, XDG_VIDEOS_DIR on Linux
	 */
	std::filesystem::path getVideos();
}

#endif //USERDIRECTORIES_H