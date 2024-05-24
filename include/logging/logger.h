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
     * @brief A model of an object that logs application messages.
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
         */
        Logger(const std::filesystem::path& path, LogLevel minimum = LogLevel::Debug);
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
