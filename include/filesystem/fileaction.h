#ifndef FILEACTION_H
#define FILEACTION_H

namespace Nickvision::Filesystem
{
    /**
     * @brief Actions that cause a file system object to change. 
     */
    enum class FileAction
    {
        Added = 1,
        Removed,
        Modified,
        Renamed
    };
}

#endif //FILEACTION_H