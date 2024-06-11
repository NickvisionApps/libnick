#include "network/webclient.h"
#include <fstream>
#include <sstream>

namespace Nickvision::Network
{
    bool WebClient::m_initialized{ false };
    unsigned int WebClient::m_instanceCount{ 0 };

    WebClient::WebClient()
    {
        if(!m_initialized)
        {
            curl_global_init(CURL_GLOBAL_ALL);
            m_initialized = true;
        }
        m_instanceCount++;
    }

    WebClient::~WebClient()
    {
        m_instanceCount--;
        if(m_initialized && m_instanceCount == 0)
        {
            curl_global_cleanup();
            m_initialized = false;
        }
    }

    bool WebClient::getWebsiteExists(const std::string& url) const
    {
        if(url.empty())
        {
            return false;
        }
        CurlEasy curl{ url };
        curl.setNoBody(true);
        return curl.perform() == CURLE_OK;
    }

    std::string WebClient::fetchJson(const std::string& url) const
    {
        if(url.empty())
        {
            return "";
        }
        CurlEasy curl{ url };
        std::stringstream out;
        curl.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/120.0");
        curl.setHeaders({ "Content-Type: application/json" });
        curl.setStream(&out);
        return curl.perform() == CURLE_OK ? out.str() : "";
    }

    bool WebClient::downloadFile(const std::string& url, const std::filesystem::path& path, const CurlProgressFunction& progress, bool overwrite) const
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