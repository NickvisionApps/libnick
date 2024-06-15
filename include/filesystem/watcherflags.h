#ifndef WATCHERFLAGS_H
#define WATCHERFLAGS_H

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

    constexpr WatcherFlags operator~(WatcherFlags a)
    {
        return static_cast<WatcherFlags>(~static_cast<int>(a));
    }

    constexpr WatcherFlags operator|(WatcherFlags a, WatcherFlags b)
    {
        return static_cast<WatcherFlags>(static_cast<int>(a) | static_cast<int>(b));
    }

    constexpr WatcherFlags operator&(WatcherFlags a, WatcherFlags b)
    {
        return static_cast<WatcherFlags>(static_cast<int>(a) & static_cast<int>(b));
    }

    constexpr WatcherFlags operator^(WatcherFlags a, WatcherFlags b)
    {
        return static_cast<WatcherFlags>(static_cast<int>(a) ^ static_cast<int>(b));
    }

    inline WatcherFlags& operator|=(WatcherFlags& a, WatcherFlags b)
    {
        return reinterpret_cast<WatcherFlags&>(reinterpret_cast<int&>(a) |= static_cast<int>(b));
    }

    inline WatcherFlags& operator&=(WatcherFlags& a, WatcherFlags b)
    {
        return reinterpret_cast<WatcherFlags&>(reinterpret_cast<int&>(a) &= static_cast<int>(b));
    }

    inline WatcherFlags& operator^=(WatcherFlags& a, WatcherFlags b)
    {
        return reinterpret_cast<WatcherFlags&>(reinterpret_cast<int&>(a) ^= static_cast<int>(b));
    }
}

#endif