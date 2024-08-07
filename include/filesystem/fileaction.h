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
 * Actions that cause a file system object to change.
 */

#ifndef FILEACTION_H
#define FILEACTION_H

namespace Nickvision::Filesystem
{
    /**
     * @brief Actions that cause a file system object to change. 
     */
    enum class FileAction
    {
        Added = 1, ///< A file was added to the file system object.
        Removed, ///< A file was removed from the file system object.
        Modified, ///< A file was modified in the file system object.
        Renamed ///< A file was renamed in the file system object.
    };
}

#endif //FILEACTION_H