#include "logging/logger.h"
#include <fstream>
#include <iostream>
#include <sstream>

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
        m_stream{ nullptr }
    {

    }

    Logger::Logger(const std::filesystem::path& path, LogLevel minimum)
        : m_minimum{ minimum },
        m_path{ path },
        m_stream{ std::make_shared<std::ofstream>(m_path, std::ios::out | std::ios::app) }
    {

    }

    const std::filesystem::path& Logger::getPath() const
    {
        return m_path;
    }

    void Logger::log(LogLevel level, const std::string& message, const std::source_location& source) const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(level < m_minimum)
        {
            return;
        }
        //Make log
        std::stringstream builder;
        builder << "[" << logLevelToString(level) << "] ";
        builder << "file: " << source.file_name() << "(" << source.line() << ":" << source.column() << ") `" << source.function_name() << "`: ";
        builder << message << std::endl;
        std::string log{ builder.str() };
        //Save log to file
        if(m_stream)
        {
            *m_stream << log << std::endl;
        }
        //Print log to terminal
        switch(level)
        {
        case LogLevel::Debug:
        case LogLevel::Info:
            std::cout << log << std::endl;
        default:
            std::cerr << log << std::endl;
        }
    }
}
