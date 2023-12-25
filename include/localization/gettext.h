#ifndef GETTEXT_H
#define GETTEXT_H

#include <libintl.h>

#ifndef _(String)
#define _(String) gettext (String)
#endif

namespace Nickvision::Aura::Localization
{
	bool init();
}

#endif //GETTEXT_H