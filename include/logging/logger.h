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
    class Logger
    {
    public:
        /**
         * @brief Constructs a Logger.
         * @brief All messages are sent to std::err.
         * @param minimum The minimum supported LogLevel
         */
        Logger(LogLevel minimum = LogLevel::Debug);
        /**
         * @brief Constructs a Logger.
         * @brief All messages are saved to the provided file.
         * @param path The path to the log file
         * @param minimum The minimum supported LogLevel
         */
        Logger(const std::filesystem::path& path, LogLevel minimum = LogLevel::Debug);
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
        std::shared_ptr<std::ostream> m_stream;
    };
}

#endif //LOGGER_H
