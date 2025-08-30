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
 * An event argument for when a file system object is changed.
 */

#ifndef FILESYSTEMCHANGEDEVENTARGS_H
#define FILESYSTEMCHANGEDEVENTARGS_H

#include <filesystem>
#include "fileaction.h"
#include "events/eventargs.h"

namespace Nickvision::Filesystem
{
    /**
     * @brief An event argument for when a file system object is changed.
     */
    class FileSystemChangedEventArgs : public Events::EventArgs
    {
    public:
        /**
         * @brief Constructs a FileSystemChangedEventArgs.
         * @param path The path of the file/folder that changed
         * @param why The action that caused the file system object to change
         */
        FileSystemChangedEventArgs(const std::filesystem::path& path, FileAction why) noexcept;
        FileSystemChangedEventArgs(const FileSystemChangedEventArgs&) noexcept = default;
        FileSystemChangedEventArgs(FileSystemChangedEventArgs&&) noexcept = default;
        /**
         * @brief Gets the path of the changed file system object system object.
         * @return The path of the changed file/folder
         */
        const std::filesystem::path& getPath() const noexcept;
        /**
         * @brief Gets the action that caused the file system object to change.
         * @return The action that caused the file system object to change
         */
        FileAction getWhy() const noexcept;
        FileSystemChangedEventArgs& operator=(const FileSystemChangedEventArgs&) noexcept = default;
        FileSystemChangedEventArgs& operator=(FileSystemChangedEventArgs&&) noexcept = default;

    private:
        std::filesystem::path m_path;
        FileAction m_why;
    };
}

#endif //FILESYSTEMCHANGEDEVENTARGS_H