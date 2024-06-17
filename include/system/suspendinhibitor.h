#ifndef SUSPENDINHIBITOR_H
#define SUSPENDINHIBITOR_H

#include <mutex>

namespace Nickvision::System
{
    /**
     * @brief An object to prevent the system from suspending.
     */
    class SuspendInhibitor
    {
    public:
        /**
         * @brief Constructs a SuspendInhibitor.
         * @brief This will not inhibit the system from suspending until the inhibit() function is called.
         */
        SuspendInhibitor();
        /**
         * @brief Copies a SuspendInhibitor object.
         * @param database The object to move
         */
        SuspendInhibitor(const SuspendInhibitor& inhibitor);
        /**
         * @brief Moves a SuspendInhibitor object.
         * @param database The object to move
         */
        SuspendInhibitor(SuspendInhibitor&& inhibitor) noexcept;
        /**
         * @brief Destructs a SuspendInhibitor.
         * @brief If the system is being inhibited, it will uninhibit.
         */
        ~SuspendInhibitor();
        /**
         * @brief Gets whether or not the system is being inhibited from suspending.
         * @return True if inhibiting, else false
         */
        bool isInhibiting() const;
        /**
         * @brief Inhibits the system from suspending.
         * @return True if successful, else false
         */
        bool inhibit();
        /**
         * @brief Uninhibits the system from suspending.
         * @return True if successful, else false
         */
        bool uninhibit();
        /**
         * @brief Copies a SuspendInhibitor object.
         * @param database The SuspendInhibitor to copy
         * @return this
         */
        SuspendInhibitor& operator=(const SuspendInhibitor& inhibitor);
        /**
         * @brief Moves a SuspendInhibitor object.
         * @param database The SuspendInhibitor to move
         * @return this
         */
        SuspendInhibitor& operator=(SuspendInhibitor&& inhibitor) noexcept;

    private:
        mutable std::mutex m_mutex;
        bool m_inhibiting;
#ifdef __linux__
        unsigned int m_cookie;
#endif
    };
}

#endif //SUSPENDINHIBITOR_H