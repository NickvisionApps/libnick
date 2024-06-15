#ifndef EVENT_H
#define EVENT_H

#include <algorithm>
#include <functional>
#include <mutex>
#include <type_traits>
#include <vector>
#include "eventargs.h"

namespace Nickvision::Events
{
    template<typename T>
    concept DerivedEventArgs = std::is_base_of_v<EventArgs, T>;

    /**
     * @brief The ID of a handler for an event.
     */
    enum class HandlerId : size_t {};

    /**
     * @brief An event that can have handlers subscribe to it, which in turn will be called when the event is invoked.
     * @tparam T Derived type of EventArgs
     */
    template <DerivedEventArgs T>
    class Event
    {
    public:
        /**
         * @brief Constructs an Event.
         */
        Event() = default;
        /**
         * @brief Constructs an Event via copy.
         * @param e The object to copy
         */
        Event(const Event& e)
        { 
            std::lock_guard<std::mutex> lock{ e.m_mutex };
            m_handlers = e.m_handlers;
        }
        /**
         * @brief Constructs an Event via move.
         * @param e The object to move
         */
        Event(Event&& e) noexcept
        {
            std::lock_guard<std::mutex> lock{ e.m_mutex };
            m_handlers = std::move(e.m_handlers);
        }
        /**
         * @brief Subscribes a handler to the event.
         * @param handler The handler function
         * @return The handler id
         */
        HandlerId subscribe(const std::function<void(const T&)>& handler)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            m_handlers.push_back(handler);
            return HandlerId{ m_handlers.size() - 1 };
        }
        /**
         * @brief Unsubscribes a handler from the event.
         * @param id The handler id
         */
        void unsubscribe(HandlerId id)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            if (static_cast<size_t>(id) < 0 || static_cast<size_t>(id) >= m_handlers.size())
            {
                return;
            }
            m_handlers.erase(m_handlers.begin() + static_cast<size_t>(id));
        }
        /**
         * @brief Invokes the event, calling all handlers.
         * @param param The parameter to pass to the handlers
         */
        void invoke(const T& param) const
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            for (const std::function<void(const T&)>& handler : m_handlers)
            {
                handler(param);
            }
        }
        /**
         * @brief Subscribes a handler to the event.
         * @param handler The handler function
         * @return The handler id
         */
        HandlerId operator+=(const std::function<void(const T&)>& handler)
        {
            return subscribe(handler);
        }
        /**
         * @brief Unsubscribes a handler from the event.
         * @param id The handler id
         */
        void operator-=(HandlerId id)
        {
            unsubscribe(id);
        }
        /**
         * @brief Invokes the event, calling all handlers.
         * @param param The parameter to pass to the handlers
         */
        void operator()(const T& param)
        {
            invoke(param);
        }
        /**
         * @brief Copies an Event.
         * @param e The Event to copy
         * @return this
         */
        Event& operator=(const Event& e)
        {
            if (this != &e)
            {
                std::lock_guard<std::mutex> lock{ m_mutex };
                std::lock_guard<std::mutex> lock2{ e.m_mutex };
                m_handlers = e.m_handlers;
            }
            return *this;
        }
        /**
         * @brief Moves an Event.
         * @param e The Event to move
         * @return this
         */
        Event& operator=(Event&& e) noexcept
        {
            if (this != &e)
            {
                std::lock_guard<std::mutex> lock{ m_mutex };
                std::lock_guard<std::mutex> lock2{ e.m_mutex };
                m_handlers = std::move(e.m_handlers);
            }
            return *this;
        }

    private:
        mutable std::mutex m_mutex;
        std::vector<std::function<void(const T&)>> m_handlers;
    };
}

#endif //EVENT_H