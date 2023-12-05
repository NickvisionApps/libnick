#include "update/updater.h"
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <json/json.h>
#include "aura.h"
#include "userdirectories.h"
#include "helpers/stringhelpers.h"
#include "helpers/webhelpers.h"
#ifdef _WIN32
#include <windows.h>
#endif

namespace Nickvision::Aura::Update
{
	Updater::Updater()
		: m_latestStableReleaseId{ -1 },
		m_latestPreviewReleaseId{ -1 }
	{
		if (!WebHelpers::getIsValidWebsite(Aura::getActive().getAppInfo().getSourceRepo()))
		{
			throw std::invalid_argument("The source repo of the active Aura::AppInfo is invalid.");
		}
		std::vector<std::string> fields = StringHelpers::split(Aura::getActive().getAppInfo().getSourceRepo(), "/");
		m_repoOwner = fields[3];
		m_repoName = fields[4];
	}

	Version Updater::fetchCurrentStableVersion()
	{
		return fetchCurrentVersion(VersionType::Stable);
	}

	Version Updater::fetchCurrentPreviewVersion()
	{
		return fetchCurrentVersion(VersionType::Preview);
	}

	bool Updater::windowsUpdate(VersionType versionType)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
#ifdef _WIN32
		if (versionType == VersionType::Stable ? m_latestStableReleaseId == -1 : m_latestPreviewReleaseId == -1)
		{
			return false;
		}
		std::string release = WebHelpers::fetchJsonString("https://api.github.com/repos/" + m_repoOwner + "/" + m_repoName + "/releases/" + std::to_string(versionType == VersionType::Stable ? m_latestStableReleaseId : m_latestPreviewReleaseId));
		if (!release.empty())
		{
			Json::Value root;
			Json::Reader reader;
			if (reader.parse(release, root, false))
			{
				for (const Json::Value& asset : root["assets"])
				{
					std::string name{ asset.get("name", "").asString() };
					std::string nameLower{ name };
					std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), [](unsigned char c) { return std::tolower(c); });
					if (nameLower.find("setup.exe") != std::string::npos)
					{
						std::filesystem::path setup{ UserDirectories::getApplicationCache() / name };
						if (WebHelpers::downloadFile(asset.get("browser_download_url", "").asString(), setup))
						{
							std::string cmd{ "\"" + setup.string() + "\"" };
							ShellExecute(nullptr, "open", cmd.c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
							exit(0);
							return true;
						}
					}
				}
			}
		}
#endif
		return false;
	}

	Version Updater::fetchCurrentVersion(VersionType versionType)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		std::string releases = WebHelpers::fetchJsonString("https://api.github.com/repos/" + m_repoOwner + "/" + m_repoName + "/releases");
		if (!releases.empty())
		{
			Json::Value root;
			Json::Reader reader;
			if (reader.parse(releases, root, false))
			{
				for (const Json::Value& release : root)
				{
					std::string version{ release.get("tag_name", "").asString() };
					if (versionType == VersionType::Stable && version.find('-') == std::string::npos)
					{
						m_latestStableReleaseId = release.get("id", -1).asInt();
						return version;
					}
					if (versionType == VersionType::Preview && version.find('-') != std::string::npos)
					{
						m_latestPreviewReleaseId = release.get("id", -1).asInt();
						return version;
					}
				}
			}
		}
		return {};
	}
}