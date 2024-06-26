#ifndef USERDIRECTORIES_H
#define USERDIRECTORIES_H

#include <filesystem>
#include <string>
#include "userdirectory.h"

namespace Nickvision::Filesystem::UserDirectories
{
    /*
     * @brief Gets the path of a user directory.
     * @brief Supports XDG directories on Linux.
     * @param directory The UserDirectory to get the path of
     * @param appName An optional application name to use for application specific directories
     * @return The path of the directory
     */
    std::filesystem::path get(UserDirectory directory, const std::string& appName = "");
}

#endif //USERDIRECTORIES_H