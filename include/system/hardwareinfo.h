#ifndef HARDWAREINFO_H
#define HARDWAREINFO_H

namespace Nickvision::System::HardwareInfo
{
    /**
     * @brief Gets the number of logical processors (CPU) on the system.
     * @return The number of logical processors
     */
    unsigned int getNumberOfProcessors();
    /**
     * @brief Gets the amount of RAM in the system in bytes.
     * @return The amount of RAM in bytes
     */
    unsigned long long getTotalRamSize();
    /**
     * @brief Gets the amount of RAM free in the system in bytes.
     * @return The amount of free RAM in bytes
     */
    unsigned long long getFreeRamSize();
}

#endif //HARDWAREINFO_H
