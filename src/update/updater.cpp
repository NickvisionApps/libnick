#include "update/updater.h"
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <boost/json.hpp>
#include "filesystem/userdirectories.h"
#include "helpers/stringhelpers.h"
#include "network/web.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace Nickvision::Filesystem;
using namespace Nickvision::Helpers;
using namespace Nickvision::Network;

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
        boost::json::value root = Web::getJson("https://api.github.com/repos/" + m_repoOwner + "/" + m_repoName + "/releases");
        if (!root.is_array())
        {
            return {};
        }
        std::string stableVersion;
        std::string previewVersion;
        for (const boost::json::value& release : root.as_array())
        {
            if(!release.is_object())
            {
                continue;
            }
            boost::json::object releaseObject = release.as_object();
            const boost::json::value& tagNameValue{ releaseObject["tag_name"] };
            if (!tagNameValue.is_string())
            {
                return {};
            }
            std::string version{ tagNameValue.as_string() };
            size_t splitCount{ StringHelpers::split(version, ".", false).size() };
            const boost::json::value& id{ releaseObject["id"] };
            if (stableVersion.empty() && versionType == VersionType::Stable && version.find('-') == std::string::npos && splitCount == 3)
            {
                m_latestStableReleaseId = id.is_int64() ? static_cast<int>(id.as_int64()) : -1;
                stableVersion = version;
            }
            if (previewVersion.empty() && versionType == VersionType::Preview && (version.find('-') != std::string::npos || splitCount == 4))
            {
                m_latestPreviewReleaseId = id.is_int64() ? static_cast<int>(id.as_int64()) : -1;
                previewVersion = version;
            }
            if (!stableVersion.empty() && !previewVersion.empty())
            {
                break;
            }
        }
        if(versionType == VersionType::Stable)
        {
            return stableVersion.empty() ? Version() : Version(stableVersion);
        }
        else if(versionType == VersionType::Preview)
        {
            return previewVersion.empty() ? Version() : Version(previewVersion);
        }
        return Version();
    }

    bool Updater::downloadUpdate(VersionType versionType, const std::filesystem::path& path, const std::string& assetName, bool exactMatch, const cpr::ProgressCallback& progress)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if (versionType == VersionType::Stable ? m_latestStableReleaseId == -1 : m_latestPreviewReleaseId == -1)
        {
            return false;
        }
        boost::json::value root = Web::getJson("https://api.github.com/repos/" + m_repoOwner + "/" + m_repoName + "/releases/" + std::to_string(versionType == VersionType::Stable ? m_latestStableReleaseId : m_latestPreviewReleaseId));
        if (!root.is_object())
        {
            return false;
        }
        boost::json::object rootObject = root.as_object();
        const boost::json::value& assets{ rootObject["assets"] };
        if (!assets.is_array())
        {
            return false;
        }
        for (const boost::json::value& asset : assets.as_array())
        {
            if (!asset.is_object())
            {
                continue;
            }
            boost::json::object assetObject = asset.as_object();
            const boost::json::value& nameValue{ assetObject["name"] };
            if(!nameValue.is_string())
            {
                return false;
            }
            std::string name{ nameValue.as_string() };
            if ((exactMatch && StringHelpers::lower(name) == StringHelpers::lower(assetName)) || (!exactMatch && StringHelpers::lower(name).find(StringHelpers::lower(assetName)) != std::string::npos))
            {
                const boost::json::value& urlValue{ assetObject["browser_download_url"] };
                if (urlValue.is_string() && Web::downloadFile(urlValue.as_string().c_str(), path, progress))
                {
                    return true;
                }
                return false;
            }
        }
        return false;
    }

#ifdef _WIN32
    bool Updater::windowsUpdate(VersionType versionType, const cpr::ProgressCallback& progress)
    {
        std::filesystem::path setupPath{ UserDirectories::get(UserDirectory::Cache) / (m_repoOwner + "_" + m_repoName + "_Setup.exe") };
        if(downloadUpdate(versionType, setupPath, "setup.exe", false, progress))
        {
            CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
            if (reinterpret_cast<INT_PTR>(ShellExecuteA(nullptr, "open", setupPath.string().c_str(), nullptr, nullptr, SW_SHOWDEFAULT)) > 32)
            {
                return true;
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
