/**
 * @file
 * @author Nicholas Logozzo <nlogozzo225@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * An object for making easy curl requests.
 */

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
         * @brief Gets whether or not to include the body in the response.
         * @return True to exclude the body, else false
         */
        bool getNoBody() const;
        /**
         * @brief Sets whether or not to include the body in the response.
         * @param noBody True to exclude the body, else false
         */
        void setNoBody(bool noBody);
        /**
         * @brief Gets the headers to include in the request.
         * @return The headers to include
         */
        const std::vector<std::string>& getHeaders() const;
        /**
         * @brief Sets the headers to include in the request.
         * @param headers The headers to include
         * @throw std::runtime_error If curl fails to append headers to the list
         */
        void setHeaders(const std::vector<std::string>& headers);
        /**
         * @brief Gets the user agent to use in the request.
         * @return The user agent to use
         */
        const std::string& getUserAgent() const;
        /**
         * @brief Sets the user agent to use in the request.
         * @param userAgent The user agent to use
         */
        void setUserAgent(const std::string& userAgent);
        /**
         * @brief Gets the stream to write the response to.
         * @return The stream to write the response to
         */
        std::basic_ostream<char>* getStream() const;
        /**
         * @brief Sets the stream to write the response to.
         * @param stream The stream to write the response to
         */
        void setStream(std::basic_ostream<char>* stream);
        /**
         * @brief Gets the progress function to use during the request.
         * @return The progress function to use
         */
        const CurlProgressFunction& getProgressFunction() const;
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
         * @throw std::runtime_error If curl fails to initialize
         */
        void init();
        CURL* m_curl;
        struct curl_slist* m_headersList;
        std::string m_url;
        bool m_noBody;
        std::vector<std::string> m_headers;
        std::string m_userAgent;
        std::basic_ostream<char>* m_stream;
        CurlProgressFunction m_progress;
    };
}

#endif //CURLEASY_H