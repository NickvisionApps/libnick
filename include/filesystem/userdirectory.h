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
 * Supported user directories.
 */

#ifndef USERDIRECTORY_H
#define USERDIRECTORY_H

namespace Nickvision::Filesystem
{
    /**
     * @brief Supported user directories.
     */
    enum class UserDirectory
    {
        Home, ///< The home directory.
        Config, ///< The configuration directory.
        ApplicationConfig, ///< The application configuration directory.
        Cache, ///< The cache directory.
        ApplicationCache, ///< The application cache directory.
        LocalData, ///< The local data directory.
        ApplicationLocalData, ///< The application local data directory.
        Desktop, ///< The desktop directory.
        Documents, ///< The documents directory.
        Downloads, ///< The downloads directory.
        Music, ///< The music directory.
        Pictures, ///< The pictures directory.
        Templates, ///< The templates directory.
        Videos ///< The videos directory.
    };
}

#endif //USERDIRECTORY_H