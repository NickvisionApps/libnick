#ifndef GETTEXT_H
#define GETTEXT_H

#include <libintl.h>

#define GETTEXT_CONTEXT_SEPARATOR "\004"
#define _(String) gettext(String)
#define _n(String, StringPlural, N) ngettext(String, StringPlural, N)
#define _p(Context, String) ::Nickvision::Aura::Localization::pgettext(Context GETTEXT_CONTEXT_SEPARATOR String, String)
#define _pn(Context, String, StringPlural, N) ::Nickvision::Aura::Localization::pngettext(Context GETTEXT_CONTEXT_SEPARATOR String, String, StringPlural, N)

namespace Nickvision::Aura::Localization
{
	bool init();
	const char* pgettext(const char* context, const char* msg);
	const char* pngettext(const char* context, const char* msg, const char* msgPlural, unsigned long n);
}

#endif //GETTEXT_H