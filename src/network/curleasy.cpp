#include "network/curleasy.h"
#include <stdexcept>

namespace Nickvision::Network
{
    static size_t writeDataCallback(char* ptr, size_t size, size_t nmemb, void* data)
    {
        std::basic_ostream<char>* stream{ static_cast<std::basic_ostream<char>*>(data) };
        stream->write(ptr, size * nmemb);
        return size * nmemb;
    }

    static int progressCallback(void* data, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
    {
        CurlProgressFunction& func{ *(static_cast<CurlProgressFunction*>(data)) };
        return func(dltotal, dlnow, ultotal, ulnow);
    }

    CurlEasy::CurlEasy(const std::string& url)
        : m_curl{ curl_easy_init() },
        m_headersList{ nullptr },
        m_url{ url }
    {
        init(url);
    }
    
    CurlEasy::~CurlEasy()
    {
        curl_easy_cleanup(m_curl);
        if(m_headersList)
        {
            curl_slist_free_all(m_headersList);
        }
    }

    const std::string& CurlEasy::getUrl() const
    {
        return m_url;
    }

    void CurlEasy::setUrl(const std::string& url)
    {
        m_url = url;
        curl_easy_setopt(m_curl, CURLOPT_URL, m_url.c_str());
    }

    void CurlEasy::setNoBody(bool value)
    {
        curl_easy_setopt(m_curl, CURLOPT_NOBODY, value);
    }

    void CurlEasy::setHeaders(const std::vector<std::string>& headers)
    {
        for(const std::string& header : headers)
        {
            struct curl_slist* temp{ curl_slist_append(m_headersList, header.c_str()) };
            if(!temp)
            {
                throw std::runtime_error("Failed to append header to list");
            }
            m_headersList = temp;
        }
        curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headersList);
    }

    void CurlEasy::setUserAgent(const std::string& userAgent)
    {
        curl_easy_setopt(m_curl, CURLOPT_USERAGENT, userAgent.c_str());
    }

    void CurlEasy::setStream(std::basic_ostream<char>* stream)
    {
        if(stream)
        {
            curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, stream);
            curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeDataCallback);
        }
        else
        {
            curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, nullptr);
            curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, nullptr);
        }
    }

    void CurlEasy::setProgressFunction(const CurlProgressFunction& progress)
    {
        m_progress = progress;
        if(m_progress)
        {
            curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, false);
            curl_easy_setopt(m_curl, CURLOPT_XFERINFODATA, &m_progress);
            curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION, progressCallback);
        }
        else
        {
            curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, true);
            curl_easy_setopt(m_curl, CURLOPT_XFERINFODATA, nullptr);
            curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION, nullptr);
        }
    }

    void CurlEasy::reset(const std::string& url)
    {
        if(m_headersList)
        {
            curl_slist_free_all(m_headersList);
            m_headersList = nullptr;
        }
        m_progress = {};
        curl_easy_reset(m_curl);
        init(url);
    }

    CURLcode CurlEasy::perform()
    {
        return curl_easy_perform(m_curl);
    }

    void CurlEasy::init(const std::string& url)
    {
        if(!m_curl)
        {
            throw std::runtime_error("Failed to initialize curl");
        }
        if(!m_url.empty())
        {
            curl_easy_setopt(m_curl, CURLOPT_URL, m_url.c_str());
        }
        curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, true);
#ifdef _WIN32
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0);
#endif
    }
}