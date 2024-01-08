#ifndef EVENTARGS_H
#define EVENTARGS_H

namespace Nickvision::Events
{
	/**
	 * @brief A base class for event arguments.
	 */
	class EventArgs
	{
	public:
		/**
		 * @brief Constructs an EventArgs.
		 */
		EventArgs() noexcept = default;
	};
}

#endif //EVENTARGS_H