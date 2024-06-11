#include "update/updater.h"
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <json/json.h>
#include "app/aura.h"
#include "filesystem/userdirectories.h"
#include "helpers/stringhelpers.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace Nickvision::App;
using namespace Nickvision::Filesystem;
using namespace Nickvision::Helpers;

namespace Nickvision::Update
{
    Updater::Updater(std::string githubRepoUrl)
        : m_latestStableReleaseId{ -1 },
        m_latestPreviewReleaseId{ -1 }
    {
        if(githubRepoUrl.empty())
        {
            throw std::invalid_argument("The url is not a valid formatted GitHub repo url.");
        }
        try
        {
            std::vector<std::string> fields{ StringHelpers::split(githubRepoUrl, "/") };
            m_repoOwner = fields[3];
            m_repoName = fields[4];
        }
        catch (...)
        {
            throw std::invalid_argument("The url is not a valid formatted GitHub repo url.");
        }
    }

    Updater::Updater(const Updater& u)
    {
        std::lock_guard<std::mutex> lock{ u.m_mutex };
        m_repoOwner = u.m_repoOwner;
        m_repoName = u.m_repoName;
        m_latestStableReleaseId = u.m_latestStableReleaseId;
        m_latestPreviewReleaseId = u.m_latestPreviewReleaseId;
    }

    Updater::Updater(Updater&& u) noexcept
    {
        std::lock_guard<std::mutex> lock{ u.m_mutex };
        m_repoOwner = std::move(u.m_repoOwner);
        m_repoName = std::move(u.m_repoName);
        m_latestStableReleaseId = std::move(u.m_latestStableReleaseId);
        m_latestPreviewReleaseId = std::move(u.m_latestPreviewReleaseId);
    }

    Version Updater::fetchCurrentVersion(VersionType versionType)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        std::string releases{ m_webClient.fetchJson("https://api.github.com/repos/" + m_repoOwner + "/" + m_repoName + "/releases") };
        if (!releases.empty())
        {
            Json::Value root;
            Json::Reader reader;
            if (reader.parse(releases, root, false))
            {
                for (const Json::Value& release : root)
                {
                    std::string version{ release.get("tag_name", "NULL").asString() };
                    if (version == "NULL")
                    {
                        return {};
                    }
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

#ifdef _WIN32
    bool Updater::windowsUpdate(VersionType versionType)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if (versionType == VersionType::Stable ? m_latestStableReleaseId == -1 : m_latestPreviewReleaseId == -1)
        {
            return false;
        }
        std::string release{ m_webClient.fetchJson("https://api.github.com/repos/" + m_repoOwner + "/" + m_repoName + "/releases/" + std::to_string(versionType == VersionType::Stable ? m_latestStableReleaseId : m_latestPreviewReleaseId)) };
        if (!release.empty())
        {
            Json::Value root;
            Json::Reader reader;
            if (reader.parse(release, root, false))
            {
                for (const Json::Value& asset : root.get("assets", {}))
                {
                    std::string name{ asset.get("name", "").asString() };
                    if (StringHelpers::lower(name).find("setup.exe") != std::string::npos)
                    {
                        std::filesystem::path setupPath{ UserDirectories::getCache() / name };
                        std::wstring quotedSetupPath{ L"\"" + setupPath.wstring() + L"\""};
                        if (m_webClient.downloadFile(asset.get("browser_download_url", "").asString(), setupPath))
                        {
                            if ((INT_PTR)ShellExecuteW(nullptr, L"open", quotedSetupPath.c_str(), nullptr, nullptr, SW_SHOWDEFAULT) > 32)
                            {
                                return true;
                            }
                        }
                        return false;
                    }
                }
            }
        }
        return false;
    }
#endif

    Updater& Updater::operator=(const Updater& u)
    {
        if (this != &u)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            std::lock_guard<std::mutex> lock2{ u.m_mutex };
            m_repoOwner = u.m_repoOwner;
            m_repoName = u.m_repoName;
            m_latestStableReleaseId = u.m_latestStableReleaseId;
            m_latestPreviewReleaseId = u.m_latestPreviewReleaseId;
        }
        return *this;
    }

    Updater& Updater::operator=(Updater&& u) noexcept
    {
        if (this != &u)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            std::lock_guard<std::mutex> lock2{ u.m_mutex };
            m_repoOwner = std::move(u.m_repoOwner);
            m_repoName = std::move(u.m_repoName);
            m_latestStableReleaseId = std::move(u.m_latestStableReleaseId);
            m_latestPreviewReleaseId = std::move(u.m_latestPreviewReleaseId);
        }
        return *this;
    }
}