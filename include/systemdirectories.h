#ifndef SYSTEMDIRECTORIES_H
#define SYSTEMDIRECTORIES_H

#include <filesystem>
#include <vector>

namespace Nickvision::Aura::SystemDirectories
{
	/*
	 * @brief Gets a list of directories from the PATH variable
	 * @return The list of directories from PATH
	 */
	std::vector<std::filesystem::path> getPath();
	/*
	 * @brief Gets a list of directories from the XDG_CONFIG_DIRS variable
	 * @return The list of directories from XDG_CONFIG_DIRS
	 */
	std::vector<std::filesystem::path> getConfig();
	/*
	 * @brief Gets a list of directories from the XDG_DATA_DIRS variable
	 * @return The list of directories from XDG_DATA_DIRS
	 */
	std::vector<std::filesystem::path> getData();
}

#endif //SYSTEMDIRECTORIES_H