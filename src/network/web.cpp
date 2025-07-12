#include "network/web.h"
#include <fstream>
#include <sstream>
#include <cpr/session.h>
#include "system/environment.h"

using namespace Nickvision::System;

namespace Nickvision::Network
{
    static bool isResponseOk(const cpr::Response& response)
    {
        if(response.status_code == 0)
        {
            return response.error.code == cpr::ErrorCode::OK;
        }
        return response.status_code >= 100 && response.status_code < 400;
    }

    bool Web::getWebsiteExists(const std::string& url)
    {
        if(url.empty())
        {
            return false;
        }
        cpr::Session session;
        session.SetUrl({ url });
        return isResponseOk(session.Get());
    }

    boost::json::value Web::getJson(const std::string& url)
    {
        if(url.empty())
        {
            return {};
        }
        cpr::Session session;
        session.SetUrl({ url });
        switch(Environment::getOperatingSystem())
        {
        case OperatingSystem::Windows:
            session.SetUserAgent({ "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36 Edg/134.0.0.0" });
            break;
        case OperatingSystem::Linux:
            session.SetUserAgent({ "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:15.0) Gecko/20100101 Firefox/15.0.1" });
            break;
        case OperatingSystem::MacOS:
            session.SetUserAgent({ "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/18.3.1 Safari/605.1.15" });
            break;
        default:
            break;
        }
        cpr::Response response{ session.Get() };
        if(isResponseOk(response) && !response.text.empty())
        {
            return boost::json::parse(response.text);
        }
        return {};
    }

    bool Web::downloadFile(const std::string& url, const std::filesystem::path& path, const cpr::ProgressCallback& progress, bool overwrite)
    {
        if(url.empty())
        {
            return false;
        }
        if(std::filesystem::exists(path))
        {
            if(!overwrite)
            {
                return false;
            }
            std::filesystem::remove(path);
        }
        std::ofstream out{ path, std::ios::binary };
        cpr::Session session;
        session.SetUrl(url);
        if(progress.callback)
        {
            session.SetProgressCallback(progress);
        }
        return isResponseOk(session.Download(out));
    }
}
