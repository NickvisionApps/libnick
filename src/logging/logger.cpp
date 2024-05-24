#include "logging/logger.h"
#include <fstream>
#include <iostream>

namespace Nickvision::Logging
{
    static std::string logLevelToString(LogLevel level)
    {
        switch(level)
        {
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARNING";
        case LogLevel::Error:
            return "ERROR";
        case LogLevel::Critical:
            return "CRITICAL";
        default:
            return "";
        }
    }

    Logger::Logger(LogLevel minimum)
        : m_minimum{ minimum },
        m_stream{ &std::cerr, [](std::ostream*){} }
    {

    }

    Logger::Logger(const std::filesystem::path& path, LogLevel minimum)
        : m_minimum{ minimum },
        m_stream{ std::make_shared<std::ofstream>(path, std::ios::out | std::ios::app) }
    {

    }

    void Logger::log(LogLevel level, const std::string& message, const std::source_location& source)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(level < m_minimum)
        {
            return;
        }
        *m_stream << "[" << logLevelToString(level) << "] ";
        *m_stream << "file: " << source.file_name() << "(" << source.line() << ":" << source.column() << ") `" << source.function_name() << "`: ";
        *m_stream << message << std::endl;
    }
}
