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

namespace Nickvision::Filesystem
{
    /**
     * @brief Flags to describe properties of a file system object that can change.  
     */
    enum class WatcherFlags
    {
        FileName = 1,
        DirectoryName = 2,
        Attributes = 4,
        Size = 8,
        LastWrite = 16,
        LastAccess = 32
    };

    constexpr WatcherFlags operator~(WatcherFlags a)
    {
        return static_cast<WatcherFlags>(~static_cast<int>(a));
    }

    constexpr WatcherFlags operator|(WatcherFlags a, WatcherFlags b)
    {
        return static_cast<WatcherFlags>(static_cast<int>(a) | static_cast<int>(b));
    }

    constexpr WatcherFlags operator&(WatcherFlags a, WatcherFlags b)
    {
        return static_cast<WatcherFlags>(static_cast<int>(a) & static_cast<int>(b));
    }

    constexpr WatcherFlags operator^(WatcherFlags a, WatcherFlags b)
    {
        return static_cast<WatcherFlags>(static_cast<int>(a) ^ static_cast<int>(b));
    }

    inline WatcherFlags& operator|=(WatcherFlags& a, WatcherFlags b)
    {
        return reinterpret_cast<WatcherFlags&>(reinterpret_cast<int&>(a) |= static_cast<int>(b));
    }

    inline WatcherFlags& operator&=(WatcherFlags& a, WatcherFlags b)
    {
        return reinterpret_cast<WatcherFlags&>(reinterpret_cast<int&>(a) &= static_cast<int>(b));
    }

    inline WatcherFlags& operator^=(WatcherFlags& a, WatcherFlags b)
    {
        return reinterpret_cast<WatcherFlags&>(reinterpret_cast<int&>(a) ^= static_cast<int>(b));
    }
}

#endif