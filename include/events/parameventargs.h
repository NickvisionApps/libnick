#ifndef PARAMEVENTARGS_H
#define PARAMEVENTARGS_H

#include <type_traits>
#include "eventargs.h"

namespace Nickvision::Events
{
    /**
     * @brief An event argument that contains a single parameter.
     * @brief T must be copy and move constructible.
     */
    template<typename T>
    class ParamEventArgs : public EventArgs
    {
    static_assert(std::is_copy_constructible_v<T> == true);
    static_assert(std::is_move_constructible_v<T> == true);

    public:
        /**
         * @brief Constructs a ParamEventArgs.
         * @param param The parameter to store in the event args
         */
        ParamEventArgs(const T& param)
            : m_param{ param }
        {

        }
        /**
         * @brief Gets the param stored in the event args.
         * @return The param stored
         */
        const T& getParam() const
        {
            return m_param;
        }

    private:
        T m_param;
    };
}

#endif //PARAMEVENTARGS_H