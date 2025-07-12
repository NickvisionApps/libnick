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
 * Functions for working with the web.
 */

#ifndef WEB_H
#define WEB_H

#include <filesystem>
#include <string>
#include <boost/json.hpp>
#include <cpr/callback.h>

namespace Nickvision::Network::Web
{
    /**
     * @brief Gets whether or not a url points to a valid website.
     * @param url The url to check
     * @return True if valid website, else false
     */
    bool getWebsiteExists(const std::string& url);
    /**
     * @brief Makes a GET request for json from a url.
     * @param url The url of the json resource
     * @return The parsed json value
     * @return An empty json value if error
     */
    boost::json::value getJson(const std::string& url);
    /**
     * @brief Downloads a file to disk.
     * @param url The url of the file to download
     * @param path The path on disk to save the file
     * @param progress An optional function to receive progress on the download
     * @param overwrite Whether or not to overwrite existing files on disk
     */
    bool downloadFile(const std::string& url, const std::filesystem::path& path, const cpr::ProgressCallback& progress = {}, bool overwrite = true);
}

#endif //WEB_H
