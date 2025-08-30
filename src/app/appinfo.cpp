#include "app/appinfo.h"
#include <maddy/parser.h>
#include "helpers/stringhelpers.h"

using namespace Nickvision::Helpers;
using namespace Nickvision::Update;

namespace Nickvision::App
{
    AppInfo::AppInfo(const std::string& id, const std::string& name, const std::string& englishShortName) noexcept
        : m_id{ id },
        m_name{ name },
        m_englishShortName{ englishShortName }
    {
        
    }

    const std::string& AppInfo::getId() const noexcept
    {
        return m_id;
    }

    void AppInfo::setId(const std::string& id) noexcept
    {
        m_id = id;
    }

    const std::string& AppInfo::getName() const noexcept
    {
        return m_name;
    }

    void AppInfo::setName(const std::string& name) noexcept
    {
        m_name = name;
    }

    const std::string& AppInfo::getShortName() const noexcept
    {
        return m_shortName;
    }

    void AppInfo::setShortName(const std::string& shortName) noexcept
    {
        m_shortName = shortName;
    }

    const std::string& AppInfo::getEnglishShortName() const noexcept
    {
        return m_englishShortName;
    }

    void AppInfo::setEnglishShortName(const std::string& englishShortName) noexcept
    {
        m_englishShortName = englishShortName;
    }

    const std::string& AppInfo::getDescription() const noexcept
    {
        return m_description;
    }

    void AppInfo::setDescription(const std::string& description) noexcept
    {
        m_description = description;
    }

    const Version& AppInfo::getVersion() const noexcept
    {
        return m_version;
    }

    void AppInfo::setVersion(const Version& version) noexcept
    {
        m_version = version;
    }

    const std::string& AppInfo::getChangelog() const noexcept
    {
        return m_changelog;
    }

    void AppInfo::setChangelog(const std::string& changelog) noexcept
    {
        m_changelog = changelog;
        if (m_changelog.empty())
        {
            m_htmlChangelog = "";
            return;
        }
        std::stringstream markdown;
        for(const std::string& line : StringHelpers::split(StringHelpers::trim(m_changelog), "\n"))
        {
            if (line.empty())
            {
                continue;
            }
            markdown << StringHelpers::trim(line);
            markdown << std::endl;
        }
        maddy::Parser parser;
        m_htmlChangelog = parser.Parse(markdown);
    }

    const std::string& AppInfo::getHtmlChangelog() const noexcept
    {
        return m_htmlChangelog;
    }

    const std::string& AppInfo::getSourceRepo() const noexcept
    {
        return m_sourceRepo;
    }

    bool AppInfo::setSourceRepo(const std::string& sourceRepo) noexcept
    {
        if (!StringHelpers::isValidUrl(sourceRepo))
        {
            return false;
        }
        m_sourceRepo = sourceRepo;
        return true;
    }

    const std::string& AppInfo::getIssueTracker() const noexcept
    {
        return m_issueTracker;
    }

    bool AppInfo::setIssueTracker(const std::string& issueTracker) noexcept
    {
        if (!StringHelpers::isValidUrl(issueTracker))
        {
            return false;
        }
        m_issueTracker = issueTracker;
        return true;
    }

    const std::string& AppInfo::getSupportUrl() const noexcept
    {
        return m_supportUrl;
    }

    bool AppInfo::setSupportUrl(const std::string& supportUrl) noexcept
    {
        if (!StringHelpers::isValidUrl(supportUrl))
        {
            return false;
        }
        m_supportUrl = supportUrl;
        return true;
    }

    const std::string& AppInfo::getHtmlDocsStore() const noexcept
    {
        return m_htmlDocsStore;
    }

    void AppInfo::setHtmlDocsStore(const std::string& htmlDocsStore) noexcept
    {
        m_htmlDocsStore = htmlDocsStore;
    }

    std::unordered_map<std::string, std::string>& AppInfo::getExtraLinks() noexcept
    {
        return m_extraLinks;
    }

    const std::unordered_map<std::string, std::string>& AppInfo::getExtraLinks() const noexcept
    {
        return m_extraLinks;
    }

    std::unordered_map<std::string, std::string>& AppInfo::getDevelopers() noexcept
    {
        return m_developers;
    }

    const std::unordered_map<std::string, std::string>& AppInfo::getDevelopers() const noexcept
    {
        return m_developers;
    }

    std::unordered_map<std::string, std::string>& AppInfo::getDesigners() noexcept
    {
        return m_designers;
    }

    const std::unordered_map<std::string, std::string>& AppInfo::getDesigners() const noexcept
    {
        return m_designers;
    }

    std::unordered_map<std::string, std::string>& AppInfo::getArtists() noexcept
    {
        return m_artists;
    }

    const std::unordered_map<std::string, std::string>& AppInfo::getArtists() const noexcept
    {
        return m_artists;
    }

    const std::string& AppInfo::getTranslatorCredits() const noexcept
    {
        return m_translatorCredits;
    }

    void AppInfo::setTranslatorCredits(const std::string& translatorCredits) noexcept
    {
        m_translatorCredits = translatorCredits;
    }

    std::vector<std::string> AppInfo::getTranslatorNames() const noexcept
    {
        if (m_translatorCredits.empty())
        {
            return { };
        }
        std::vector<std::string> vec;
        for (const std::string& line : StringHelpers::split(m_translatorCredits, "\n"))
        {
            size_t index;
            if ((index = line.find('<')) != std::string::npos)
            {
                vec.push_back(StringHelpers::trim(line.substr(0, index)));
            }
            else if ((index = line.find("http")) != std::string::npos)
            {
                vec.push_back(StringHelpers::trim(line.substr(0, index)));
            }
            else
            {
                vec.push_back(line);
            }
        }
        return vec;
    }
}