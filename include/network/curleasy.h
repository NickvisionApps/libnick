#ifndef CURLEASY_H
#define CURLEASY_H

#include <functional>
#include <ostream>
#include <string>
#include <vector>
#include <curl/curl.h>

namespace Nickvision::Network
{
    using CurlProgressFunction = std::function<int(curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)>;

    /**
     * @brief An object for making easy curl requests.
     */
    class CurlEasy
    {
    public:
        /**
         * @brief Constructs a CurlEasy.
         * @param url The url to make requests to
         * @throw std::runtime_error If curl fails to initialize
         */
        CurlEasy(const std::string& url = "");
        /**
         * @brief Destructs a CurlEasy.
         */
        ~CurlEasy();
        /**
         * @brief Gets the url to make requests to.
         * @return The url to make requests to
         */
        const std::string& getUrl() const;
        /**
         * @brief Sets the url to make requests to.
         * @param url The url to make requests to
         */
        void setUrl(const std::string& url);
        /**
         * @brief Sets whether or not to include the body in the response.
         * @param value True to exclude the body, else false
         */
        void setNoBody(bool value);
        /**
         * @brief Sets the headers to include in the request.
         * @param headers The headers to include
         * @throw std::runtime_error If curl fails to append headers to the list
         */
        void setHeaders(const std::vector<std::string>& headers);
        /**
         * @brief Sets the user agent to use in the request.
         * @param userAgent The user agent to use
         */
        void setUserAgent(const std::string& userAgent);
        /**
         * @brief Sets the stream to write the response to.
         * @param stream The stream to write the response to
         */
        void setStream(std::basic_ostream<char>* stream);
        /**
         * @brief Sets the progress function to use during the request.
         * @param progress The progress function to use
         */
        void setProgressFunction(const CurlProgressFunction& progress);
        /**
         * @brief Resets the curl request.
         * @param url The url to make requests to
         * @throw std::runtime_error If curl fails to initialize
         */
        void reset(const std::string& url = "");
        /**
         * @brief Performs the curl request.
         * @return The code returned by the curl request
         */
        CURLcode perform();

    private:
        /**
         * @brief Initializes the curl request handle.
         * @param url The url to make requests to
         * @throw std::runtime_error If curl fails to initialize
         */
        void init(const std::string& url);
        CURL* m_curl;
        struct curl_slist* m_headersList;
        std::string m_url;
        CurlProgressFunction m_progress;
    };
}

#endif //CURLEASY_H