#include "filesystem/userdirectories.h"
#include <fstream>
#include <string>
#include "aura/aura.h"
#include "helpers/stringhelpers.h"
#ifdef _WIN32
#include <shlobj_core.h>
#elif defined(__linux__)
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#endif 

namespace Nickvision::Filesystem
{
#ifdef __linux__
	static std::filesystem::path getXDGDir(const std::string& key) noexcept
	{
		std::string var{ Aura::Aura::getEnvVar(key) };
		if (!var.empty())
		{
			return var;
		}
		std::filesystem::path dirsPath{ UserDirectories::getConfig() / "user-dirs.dirs" };
		if (!std::filesystem::exists(dirsPath))
		{
			return {};
		}
		std::ifstream dirs{ dirsPath };
		std::string line;
		while (std::getline(dirs, line))
		{
			if (line[0] == '#' || line.empty())
			{
				continue;
			}
			std::vector<std::string> pair{ StringHelpers::split(line, "=") };
			if (pair[0] == key)
			{
				if (pair[1].find("$HOME") != std::string::npos)
				{
					pair[1].replace(pair[1].find("$HOME"), 5, UserDirectories::getHome().string());
				}
				return StringHelpers::trim(pair[1], '"');
			}
		}
		return { };
	}
#endif

	std::filesystem::path UserDirectories::getHome() noexcept
	{
		std::filesystem::path result;
#ifdef _WIN32
		wchar_t* p{ nullptr };
		if (SHGetKnownFolderPath(FOLDERID_Profile, 0, nullptr, &p) == S_OK)
		{
			result = p;
		}
		CoTaskMemFree(static_cast<void*>(p));
#elif defined(__linux__)
		std::filesystem::path var{ Aura::Aura::getEnvVar("HOME") };
		result = !var.empty() ? var : getpwuid(getuid())->pw_dir;
#endif
		return result;
	}

	std::filesystem::path UserDirectories::getConfig() noexcept
	{
		std::filesystem::path result;
#ifdef _WIN32
		wchar_t* p{ nullptr };
		if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &p) == S_OK)
		{
			result = p;
		}
		CoTaskMemFree(static_cast<void*>(p));
#elif defined(__linux__)
		std::filesystem::path var{ Aura::Aura::getEnvVar("XDG_CONFIG_HOME") };
		result = !var.empty() ? var : (getHome() / ".config");
#endif
		std::filesystem::create_directories(result);
		return result;
	}

	std::filesystem::path UserDirectories::getApplicationConfig() noexcept
	{
		std::filesystem::path result = getConfig() / Aura::Aura::getActive().getAppInfo().getName();
		std::filesystem::create_directories(result);
		return result;
	}

	std::filesystem::path UserDirectories::getCache() noexcept
	{
		std::filesystem::path result;
#ifdef _WIN32
		wchar_t* p{ nullptr };
		if (SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &p) == S_OK)
		{
			result = p;
		}
		CoTaskMemFree(static_cast<void*>(p));
#elif defined(__linux__)
		std::filesystem::path var{ Aura::Aura::getEnvVar("XDG_CACHE_HOME") };
		result = !var.empty() ? var : (getHome() / ".cache");
#endif
		std::filesystem::create_directories(result);
		return result;
	}

	std::filesystem::path UserDirectories::getApplicationCache() noexcept
	{
		std::filesystem::path result = getCache() / Aura::Aura::getActive().getAppInfo().getName();
		std::filesystem::create_directories(result);
		return result;
	}

	std::filesystem::path UserDirectories::getLocalData() noexcept
	{
		std::filesystem::path result;
#ifdef _WIN32
		result = getConfig();
#elif defined(__linux__)
		std::filesystem::path var{ Aura::Aura::getEnvVar("XDG_DATA_HOME") };
		result = !var.empty() ? var : (getHome() / ".local/share");
#endif
		std::filesystem::create_directories(result);
		return result;
	}

	std::filesystem::path UserDirectories::getApplicationLocalData() noexcept
	{
		std::filesystem::path result{ getLocalData() / Aura::Aura::getActive().getAppInfo().getName() };
		std::filesystem::create_directories(result);
		return result;
	}

	std::filesystem::path UserDirectories::getRuntime() noexcept
	{
		std::filesystem::path result;
#ifdef __linux__
		std::filesystem::path var{ Aura::Aura::getEnvVar("XDG_RUNTIME_DIR") };
		result = !var.empty() ? var : (std::filesystem::path("/run/user/") / Aura::Aura::getEnvVar("UID"));
#endif
		return result;
	}

	std::filesystem::path UserDirectories::getDesktop() noexcept
	{
		std::filesystem::path result;
#ifdef _WIN32
		wchar_t* p{ nullptr };
		if (SHGetKnownFolderPath(FOLDERID_Desktop, 0, nullptr, &p) == S_OK)
		{
			result = p;
		}
		CoTaskMemFree(static_cast<void*>(p));
#elif defined(__linux__)
		result = getXDGDir("XDG_DESKTOP_DIR");
		result = result.empty() ? (getHome() / "Desktop") : result;
#endif
		std::filesystem::create_directories(result);
		return result;
	}

	std::filesystem::path UserDirectories::getDocuments() noexcept
	{
		std::filesystem::path result;
#ifdef _WIN32
		wchar_t* p{ nullptr };
		if (SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &p) == S_OK)
		{
			result = p;
		}
		CoTaskMemFree(static_cast<void*>(p));
#elif defined(__linux__)
		result = getXDGDir("XDG_DOCUMENTS_DIR");
		result = result.empty() ? (getHome() / "Documents") : result;
#endif
		std::filesystem::create_directories(result);
		return result;
	}

	std::filesystem::path UserDirectories::getDownloads() noexcept
	{
		std::filesystem::path result;
#ifdef _WIN32
		wchar_t* p{ nullptr };
		if (SHGetKnownFolderPath(FOLDERID_Downloads, 0, nullptr, &p) == S_OK)
		{
			result = p;
		}
		CoTaskMemFree(static_cast<void*>(p));
#elif defined(__linux__)
		result = getXDGDir("XDG_DOWNLOAD_DIR");
		result = result.empty() ? (getHome() / "Downloads") : result;
#endif
		std::filesystem::create_directories(result);
		return result;
	}

	std::filesystem::path UserDirectories::getMusic() noexcept
	{
		std::filesystem::path result;
#ifdef _WIN32
		wchar_t* p{ nullptr };
		if (SHGetKnownFolderPath(FOLDERID_Music, 0, nullptr, &p) == S_OK)
		{
			result = p;
		}
		CoTaskMemFree(static_cast<void*>(p));
#elif defined(__linux__)
		result = getXDGDir("XDG_MUSIC_DIR");
		result = result.empty() ? (getHome() / "Music") : result;
#endif
		std::filesystem::create_directories(result);
		return result;
	}

	std::filesystem::path UserDirectories::getPictures() noexcept
	{
		std::filesystem::path result;
#ifdef _WIN32
		wchar_t* p{ nullptr };
		if (SHGetKnownFolderPath(FOLDERID_Pictures, 0, nullptr, &p) == S_OK)
		{
			result = p;
		}
		CoTaskMemFree(static_cast<void*>(p));
#elif defined(__linux__)
		result = getXDGDir("XDG_PICTURES_DIR");
		result = result.empty() ? (getHome() / "Pictures") : result;
#endif
		std::filesystem::create_directories(result);
		return result;
	}

	std::filesystem::path UserDirectories::getPublicShare() noexcept
	{
		std::filesystem::path result;
#ifdef __linux__
		result = getXDGDir("XDG_PUBLICSHARE_DIR");
#endif
		return result;
	}

	std::filesystem::path UserDirectories::getTemplates() noexcept
	{
		std::filesystem::path result;
#ifdef _WIN32
		wchar_t* p{ nullptr };
		if (SHGetKnownFolderPath(FOLDERID_Templates, 0, nullptr, &p) == S_OK)
		{
			result = p;
		}
		CoTaskMemFree(static_cast<void*>(p));
#elif defined(__linux__)
		result = getXDGDir("XDG_TEMPLATES_DIR");
		result = result.empty() ? (getHome() / "Templates") : result;
#endif
		std::filesystem::create_directories(result);
		return result;
	}

	std::filesystem::path UserDirectories::getVideos() noexcept
	{
		std::filesystem::path result;
#ifdef _WIN32
		wchar_t* p{ nullptr };
		if (SHGetKnownFolderPath(FOLDERID_Videos, 0, nullptr, &p) == S_OK)
		{
			result = p;
		}
		CoTaskMemFree(static_cast<void*>(p));
#elif defined(__linux__)
		result = getXDGDir("XDG_VIDEOS_DIR");
		result = result.empty() ? (getHome() / "Videos") : result;
#endif
		std::filesystem::create_directories(result);
		return result;
	}
}