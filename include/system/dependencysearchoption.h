#ifndef DEPENDENCYSEARCHOPTION_H
#define DEPENDENCYSEARCHOPTION_H

namespace Nickvision::System
{
    /**
     * @brief Search options for search for dependencies on the system
     */
    enum class DependencySearchOption
    {
        Global, // (Default) Searches first in the app's executable directory, then searches through the PATH variable
        App, // Searches in the app's executable directory only
        System, // Searches in the PATH variable only
        Local // Searches in the UserDirectory::LocalData folder
    };
}

#endif //DEPENDENCYSEARCHOPTION_H
