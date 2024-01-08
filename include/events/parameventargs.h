#ifndef PARAMEVENTARGS_H
#define PARAMEVENTARGS_H

#include "eventargs.h"

namespace Nickvision::Events
{
	/**
	 * @brief An event argument that contains a single parameter.
	 */
	template<typename T>
	class ParamEventArgs : public EventArgs
	{
	public:
		/**
		 * @brief Constructs a ParamEventArgs.
		 * @param param The parameter to store in the event args
		 */
		ParamEventArgs(const T& param) noexcept
			: m_param{ param }
		{

		}
		/**
		 * @brief Gets the param stored in the event args.
		 * @return The param stored
		 */
		const T& getParam() const noexcept
		{
			return m_param;
		}

	private:
		T m_param;
	};
}

#endif //PARAMEVENTARGS_H