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
 * Headers and common definitions for SQLite database interactions.
 */

#ifndef SQLITE_H
#define SQLITE_H

#ifndef SQLITE_HAS_CODEC
#define SQLITE_HAS_CODEC
#endif

#include <cstdint>
#include <string>
#include <type_traits>
#ifdef _WIN32
#include <sqlcipher/sqlite3.h>
#else
#include <sqlite3.h>
#endif

namespace Nickvision::Database
{
    template<typename T>
    concept SupportedSqliteValue = std::is_same_v<T, int> || std::is_same_v<T, std::int64_t> || std::is_same_v<T, double> || std::is_same_v<T, bool> || std::is_same_v<T, std::string>;
}

#endif //SQLITE_H