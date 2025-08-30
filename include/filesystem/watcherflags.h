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
 * Flags to describe properties of a file system object that can change.
 */

#ifndef WATCHERFLAGS_H
#define WATCHERFLAGS_H

#include "helpers/codehelpers.h"

namespace Nickvision::Filesystem
{
    /**
     * @brief Flags to describe properties of a file system object that can change.  
     */
    enum class WatcherFlags
    {
        FileName = 1, ///< The name of a file in the file system object has changed.
        DirectoryName = 2, ///< The name of a directory in the file system object has changed.
        Attributes = 4,  ///< The attributes of an item in the file system object have changed.
        Size = 8, ///< The size of an item in the file system object has changed.
        LastWrite = 16, ///< The last write time of an item in the file system object has changed.
        LastAccess = 32 ///< The last access time of an item in the file system object has changed.
    };

    DEFINE_ENUM_FLAGS(WatcherFlags)
}

#endif