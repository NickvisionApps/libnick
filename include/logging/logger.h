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
 * A logger of application messages.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>
#include <memory>
#include <mutex>
#include <source_location>
#include <string>
#include "loglevel.h"

namespace Nickvision::Logging
{
    /**
     * @brief A logger of application messages.
     */
    class Logger
    {
    public:
        /**
         * @brief Constructs a Logger.
         * @brief All messages are printed to standard outputs.
         * @param minimum The minimum supported LogLevel
         */
        Logger(LogLevel minimum = LogLevel::Debug);
        /**
         * @brief Constructs a Logger.
         * @brief All messages are saved to the provided file and printed to standard outputs.
         * @param path The path to the log file
         * @param minimum The minimum supported LogLevel
         * @param append Whether or not to append to the file if it already exists
         * @throw std::runtime_error Thrown if the unable to open the file
         */
        Logger(const std::filesystem::path& path, LogLevel minimum = LogLevel::Debug, bool append = true);
        /**
         * @brief Gets the path for the logs file.
         * @brief If the logger has no file attached, an empty path will be returned.
         * @return The path to the logs file
         */
        const std::filesystem::path& getPath() const; 
        /**
         * @brief Logs a message.
         * @param level LogLevel
         * @param message The message to log
         * @param source The code location of the log message
         */
        void log(LogLevel level, const std::string& message, const std::source_location& source = std::source_location::current()) const;

    private:
        mutable std::mutex m_mutex;
        LogLevel m_minimum;
        std::filesystem::path m_path;
        std::shared_ptr<std::ostream> m_stream;
    };
}

#endif //LOGGER_H
