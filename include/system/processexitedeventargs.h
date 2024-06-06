#ifndef PROCESSEXITEDEVENTARGS_H
#define PROCESSEXITEDEVENTARGS_H

#include <string>
#include "events/eventargs.h"

namespace Nickvision::System
{
    /**
     * @brief Event args for when a process has exited.
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