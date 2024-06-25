#ifndef DEPLOYMENTMODE_H
#define DEPLOYMENTMODE_H

namespace Nickvision::System
{
    /**
     * @brief Supported deployment mode values.
     */
    enum class DeploymentMode
    {
        Local,
        Flatpak,
        Snap
    };
}

#endif //DEPLOYMENTMODE_H