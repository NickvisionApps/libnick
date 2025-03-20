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
 * A managed process.
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <cstdio>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "events/event.h"
#include "processexitedeventargs.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#endif

namespace Nickvision::System
{
    /**
     * @brief A managed process.
     */
    class Process
    {
    public:
        /**
         * @brief Constructs a Process.
         * @param path The path of the process to execute
         * @param args The arguments to pass to the process
         * @param workingDir An optional working directory to use for a process
         * @throw std::runtime_error Thrown if the process could not be created
         */
        Process(const std::filesystem::path& path, const std::vector<std::string>& args = {}, const std::filesystem::path& workingDir = {});
        /**
         * @brief Destructs a Process.
         * @brief This method will wait for the process to exit if it is still running.
         */
        ~Process();
        /**
         * @brief Gets the path of the process.
         * @return The path of the process
         */
        const std::filesystem::path& getPath() const;
        /**
         * @brief Gets the event for when the process has exited.
         * @return The process exited event
         */
        Events::Event<ProcessExitedEventArgs>& exited();
        /**
         * @brief Gets whether or not the process is running.
         * @return True if running, else false
         */
        bool isRunning() const;
        /**
         * @brief Gets whether or not the process has completed.
         * @return True if completed, else false
         */
        bool hasCompleted() const;
        /**
         * @brief Gets the exit code of the process.
         * @return The exit code of the process. -1 if the process has not completed
         */
        int getExitCode() const;
        /**
         * @brief Gets the console output of the process.
         * @return The console output of the process. Empty if the process has not completed
         */
        const std::string& getOutput() const;
        /**
         * @brief Gets the amount of RAM being used by the process in bytes.
         * @return The amount of RAM used by the process
         */
        unsigned long long getRAMUsage() const;
        /**
         * @brief Gets the percent of the CPU being used by the process.
         * @return The CPU usage of the process
         */
        double getCPUUsage() const;
        /**
         * @brief Starts the process.
         * @return True if the process was started, else false
         */
        bool start();
        /**
         * @brief Kills the process.
         * @return True if the process was killed, else false
         */
        bool kill();
        /**
         * @brief Waits for the process to exit.
         * @brief This function will block until the process has exited.
         * @brief Make sure to call start() before calling this function.
         * @return The exit code of the process
         */
        int waitForExit();
        /**
         * @brief Send text to the process's console.
         * @param s The text to send
         * @return True if the text is sent, else false
         */
        bool send(const std::string& s);
        /**
         * @brief Send text to the process's console and adds the return characters.
         * @param s The command to send
         * @return True if the command is sent, else false
         */
        bool sendCommand(std::string s);

    private:
        /**
         * @brief Watches the process.
         */
        void watch();
        mutable std::mutex m_mutex;
        std::filesystem::path m_path;
        std::vector<std::string> m_args;
        std::filesystem::path m_workingDirectory;
        Events::Event<ProcessExitedEventArgs> m_exited;
        bool m_running;
        bool m_completed;
        int m_exitCode;
        std::string m_output;
        std::thread m_watchThread;
#ifdef _WIN32
        HANDLE m_childOutRead;
        HANDLE m_childOutWrite;
        HANDLE m_childInRead;
        HANDLE m_childInWrite;
        PROCESS_INFORMATION m_pi;
        HANDLE m_job;
        mutable unsigned long long m_lastProcKernelTime;
        mutable unsigned long long m_lastProcUserTime;
        mutable unsigned long long m_lastSysKernelTime;
        mutable unsigned long long m_lastSysUserTime;
#else
        int m_childOutPipes[2];
        int m_childInPipes[2];
        pid_t m_pid;
        mutable unsigned long long m_lastUserTime;
        mutable unsigned long long m_lastSystemTime;
#endif
    };
}

#endif //PROCESS_H
