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
        Home,
        Config,
        ApplicationConfig,
        Cache,
        ApplicationCache,
        LocalData,
        ApplicationLocalData,
        Desktop,
        Documents,
        Downloads,
        Music,
        Pictures,
        Templates,
        Videos
    };
}

#endif //USERDIRECTORY_H