#ifndef DOCUMENTATION_H
#define DOCUMENTATION_H

#include <string>

namespace Nickvision::Localization::Documentation
{
    /**
     * @brief Gets the url for a documentation page.
     * @brief This will be a yelp url for Linux and a website url for Windows and Linux snaps.
     * @param englishShortName The english short name of the application
     * @param htmlDocStore The html docs store url
     * @param pageName The name of the documentation page to get
     * @return The url for the documentation page
     */
    std::string getHelpUrl(const std::string& englishShortName, const std::string& htmlDocStore, const std::string& pageName);
}

#endif //DOCUMENTATION_H