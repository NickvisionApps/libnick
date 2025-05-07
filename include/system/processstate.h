#ifndef PROCESSSTATE_H
#define PROCESSSTATE_H

namespace Nickvision::System
{
    /**
     * @brief States of a process.
     */
    enum class ProcessState
    {
        Created, //The process was just created (start() not yet called)
        Running, //The process is running
        Killed, //The process was killed and is no longer running
        Paused, //The process was paused and is no longer running (continue using resume())
        Completed //The process was completed and is no longer running
    };
}

#endif //PROCESSSTATE_H
