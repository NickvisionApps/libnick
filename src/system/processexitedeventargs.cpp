#include "system/processexitedeventargs.h"

namespace Nickvision::System
{
    ProcessExitedEventArgs::ProcessExitedEventArgs(int exitCode, const std::string& output)
        : m_exitCode{ exitCode },
        m_output{ output }
    {

    }

    int ProcessExitedEventArgs::getExitCode() const
    {
        return m_exitCode;
    }

    const std::string& ProcessExitedEventArgs::getOutput() const
    {
        return m_output;
    }
}