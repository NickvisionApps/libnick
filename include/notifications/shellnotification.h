#ifndef SHELLNOTIFICATION_H
#define SHELLNOTIFICATION_H

#include "shellnotificationsenteventargs.h"

namespace Nickvision::Aura::Notifications::ShellNotification
{
	bool send(const ShellNotificationSentEventArgs& args);
}

#endif //SHELLNOTIFICATION_H