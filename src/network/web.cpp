#include "network/web.h"
#include <fstream>
#include <sstream>
#include "system/environment.h"

using namespace Nickvision::System;

namespace Nickvision::Network
{
    bool Web::getWebsiteExists(const std::string& url)
    {
        if(url.empty())
        {
            return false;
        }
        CurlEasy curl{ url };
        curl.setNoBody(true);
        return curl.perform() == CURLE_OK;
    }

    boost::json::value Web::fetchJson(const std::string& url)
    {
        if(url.empty())
        {
            return {};
        }
        CurlEasy curl{ url };
        std::stringstream out;
        switch(Environment::getOperatingSystem())
        {
        case OperatingSystem::Windows:
            curl.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36 Edg/131.0.2903.86");
            break;
        case OperatingSystem::Linux:
            curl.setUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36");
            break;
        case OperatingSystem::MacOS:
            curl.setUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 14_7_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36 Edg/131.0.2903.86");
            break;
        default:
            break;
        }
        curl.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/133.0.0.0 Safari/537.36");
        curl.setHeaders({ "Content-Type: application/json" });
        curl.setStream(&out);
        if(curl.perform() == CURLE_OK)
        {
            std::string data{ out.str() };
            if(!data.empty())
            {
                return boost::json::parse(data);
            }
        }
        return {};
    }

    bool Web::downloadFile(const std::string& url, const std::filesystem::path& path, const CurlProgressFunction& progress, bool overwrite)
    {
        if(url.empty())
        {
            return false;
        }
        if(std::filesystem::exists(path) && !overwrite)
        {
            return false;
        }
        CurlEasy curl{ url };
        std::ofstream out{ path, std::ios::binary | std::ios::trunc };
        curl.setStream(&out);
        curl.setProgressFunction(progress);
        return curl.perform() == CURLE_OK;
    }
}
