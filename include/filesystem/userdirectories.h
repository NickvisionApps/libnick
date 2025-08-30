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
 * Functions for working with user directories.
 */

#ifndef USERDIRECTORIES_H
#define USERDIRECTORIES_H

#include <filesystem>
#include <string>
#include "applicationuserdirectory.h"
#include "userdirectory.h"

namespace Nickvision::Filesystem::UserDirectories
{
    /**
     * @brief Gets the path of a user directory.
     * @brief Supports XDG directories on Linux.
     * @param directory The UserDirectory to get the path of
     * @return The path of the directory
     */
    std::filesystem::path get(UserDirectory directory) noexcept;
    /**
     * @brief Gets the path of an application user directory.
     * @brief Supports XDG directories on Linux.
     * @param directory The ApplicationUserDirectory to get the path of
     * @param appName The application name
     * @return The path of the directory
     */
    std::filesystem::path get(ApplicationUserDirectory directory, const std::string& appName) noexcept;
}

#endif //USERDIRECTORIES_H