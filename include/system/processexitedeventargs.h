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
 * An event argument for when a process has exited.
 */

#ifndef PROCESSEXITEDEVENTARGS_H
#define PROCESSEXITEDEVENTARGS_H

#include <string>
#include "events/eventargs.h"

namespace Nickvision::System
{
    /**
     * @brief An event argument for when a process has exited.
     */
    class ProcessExitedEventArgs : public Events::EventArgs
    {
    public:
        /**
         * @brief Constructs a ProcessExitedEventArgs.
         * @param exitCode The exit code of the process
         * @param output The console output of the process
         */
        ProcessExitedEventArgs(int exitCode, const std::string& output);
        /**
         * @brief Gets the exit code of the process.
         * @return The exit code of the process
         */
        int getExitCode() const;
        /**
         * @brief Gets the console output of the process.
         * @return The console output of the process
         */
        const std::string& getOutput() const;

    private:
        int m_exitCode;
        std::string m_output;
    };
}

#endif //PROCESSEXITEDEVENTARGS_H