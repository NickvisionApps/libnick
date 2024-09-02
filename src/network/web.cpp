#include "network/web.h"
#include <fstream>
#include <sstream>

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

    boost::json::object Web::fetchJson(const std::string& url)
    {
        if(url.empty())
        {
            return {};
        }
        CurlEasy curl{ url };
        std::stringstream out;
        curl.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/120.0");
        curl.setHeaders({ "Content-Type: application/json" });
        curl.setStream(&out);
        if(curl.perform() == CURLE_OK)
        {
            std::string data{ out.str() };
            if(!data.empty())
            {
                return boost::json::parse(data).as_object();
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