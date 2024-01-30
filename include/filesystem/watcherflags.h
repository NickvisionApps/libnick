#ifndef WATCHERFLAGS_H
#define WATCHERFLAGS_H

#include "enumflags.h"

namespace Nickvision::Filesystem
{
    /**
     * @brief Flags to describe properties of a file system object that can change.  
     */
    enum class WatcherFlags
    {
        FileName = 1,
        DirectoryName = 2,
        Attributes = 4,
        Size = 8,
        LastWrite = 16,
        LastAccess = 32
    };

    DEFINE_ENUM_FLAG_OPERATORS(WatcherFlags);
}

#endif