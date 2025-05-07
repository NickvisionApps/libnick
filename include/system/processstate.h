#ifndef PROCESSSTATE_H
#define PROCESSSTATE_H

namespace Nickvision::System
{
    enum class ProcessState
    {
        Created,
        Running,
        Killed,
        Paused,
        Completed
    };
}

#endif //PROCESSSTATE_H
