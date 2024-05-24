#ifndef LOGLEVEL_H
#define LOGLEVEL_H

namespace Nickvision::Logging
{
    /**
     * @brief Levels of log messages.
     */
    enum class LogLevel
    {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };
}

#endif //LOGLEVEL_H
