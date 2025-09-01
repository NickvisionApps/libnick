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
 * Functions for working with an application's documentation.
 */

#ifndef DOCUMENTATION_H
#define DOCUMENTATION_H

#include <string>

namespace Nickvision::Localization::Documentation
{
    /**
     * @brief Gets the url for a documentation page.
     * @brief This will be a yelp url for Linux and a website url for Windows and Linux snaps.
     * @param englishShortName The english short name of the application
     * @param htmlDocStore The html docs store url
     * @param pageName The name of the documentation page to get
     * @return The url for the documentation page
     */
    std::string getHelpUrl(const std::string& englishShortName, const std::string& htmlDocStore, const std::string& pageName) noexcept;
}

#endif //DOCUMENTATION_H