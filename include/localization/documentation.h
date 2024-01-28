#ifndef DOCUMENTATION_H
#define DOCUMENTATION_H

#include <string>

namespace Nickvision::Localization::Documentation
{
	/**
	 * @brief Gets the url for a documentation page.
	 * @brief This will be a yelp url for Linux and a website url for Windows and Linux snaps.
	 * @brief HtmlDocsStore should be set for Aura::getActive()::getAppInfo()
	 * @param pageName The name of the page to get the url for
	 * @return The url for the documentation page
	 */
	std::string getHelpUrl(const std::string& pageName);
}

#endif //DOCUMENTATION