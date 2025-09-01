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
 * Search options for search for dependencies on the system.
 */

#ifndef DEPENDENCYSEARCHOPTION_H
#define DEPENDENCYSEARCHOPTION_H

namespace Nickvision::System
{
    /**
     * @brief Search options for search for dependencies on the system.
     */
    enum class DependencySearchOption
    {
        Global, // (Default) Searches first in the app's executable directory, then searches through the PATH variable
        App, // Searches in the app's executable directory only
        System, // Searches in the PATH variable only
        Local // Searches in the UserDirectory::LocalData folder
    };
}

#endif //DEPENDENCYSEARCHOPTION_H
