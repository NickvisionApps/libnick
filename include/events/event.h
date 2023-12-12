#ifndef EVENT_H
#define EVENT_H

#include <algorithm>
#include <functional>
#include <mutex>
#include <type_traits>
#include <vector>
#include "eventargs.h"

namespace Nickvision::Aura::Events
{
	/**
	 * @brief An event that can have handlers subscribe to it, which in turn will be called when the event is invoked.
	 * @tparam T Derived type of EventArgs
	 */
	template <typename T> 
	class Event
	{
		static_assert(std::is_base_of_v<EventArgs, T> == true, "T must derive from EventArgs");

	public:
		/**
		 * @brief Constructs an Event.
		 */
		Event() { }
		/**
		 * @brief Constructs an Event via copy.
		 */
		Event(const Event& e) 
		{ 
			std::lock_guard<std::mutex> lock{ e.m_mutex };
			m_handlers = e.m_handlers;
		}
		/**
		 * @brief Constructs an Event via move.
		 */
		Event(Event&& e) noexcept
		{
			std::lock_guard<std::mutex> lock{ e.m_mutex };
			m_handlers = std::move(e.m_handlers);
		}
		/**
		 * @brief Subscribes a handler to the event
		 * @param handler The handler function
		 */
		void subscribe(const std::function<void(const T&)>& handler)
		{
			std::lock_guard<std::mutex> lock{ m_mutex };
			m_handlers.push_back(handler);
		}
		/**
		 * @brief Unsubscribes a handler from the event
		 * @param handler The handler function
		 */
		void unsubscribe(const std::function<void(const T&)>& handler)
		{
			std::lock_guard<std::mutex> lock{ m_mutex };
			if (m_handlers.size() == 0)
			{
				return;
			}
			auto it = std::find(m_handlers.begin(), m_handlers.end(), handler);
			if (it != m_handlers.end())
			{
				m_handlers.erase(it);
			}
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
		 * @brief Subscribes a handler to the event
		 * @param handler The handler function
		 */
		void operator+=(const std::function<void(const T&)>& handler)
		{
			subscribe(handler);
		}
		/**
		 * @brief Unsubscribes a handler from the event
		 * @param handler The handler function
		 */
		void operator-=(const std::function<void(const T&)>& handler)
		{
			unsubscribe(handler);
		}
		/**
		 * @brief Copies an Event
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
		 * @brief Moves an Event
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