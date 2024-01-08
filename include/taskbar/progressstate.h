#ifndef PROGRESSSTATE_H
#define PROGRESSSTATE_H

namespace Nickvision::Taskbar
{
	/**
	 * @brief States of progress on a taskbar button.
	 */
	enum class ProgressState
	{
		NoProgress = 0,
		Indeterminate = 1,
		Normal = 2,
		Error = 4,
		Paused = 8
	};
}

#endif //PROGRESSSTATE_H