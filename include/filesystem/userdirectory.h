#ifndef USERDIRECTORY_H
#define USERDIRECTORY_H

namespace Nickvision::Filesystem
{
    /**
     * @brief Supported user directories.
     */
    enum class UserDirectory
    {
        Home,
        Config,
        ApplicationConfig,
        Cache,
        ApplicationCache,
        LocalData,
        ApplicationLocalData,
        Desktop,
        Documents,
        Downloads,
        Music,
        Pictures,
        Templates,
        Videos
    };
}

#endif //USERDIRECTORY_H