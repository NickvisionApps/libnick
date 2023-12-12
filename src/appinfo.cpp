#include "appinfo.h"
#include <memory>
#include <sstream>
#include <maddy/parser.h>
#include "helpers/stringhelpers.h"

namespace Nickvision::Aura
{
	AppInfo::AppInfo()
	{

	}

	const std::string& AppInfo::getId() const
	{
		return m_id;
	}

	void AppInfo::setId(const std::string& id)
	{
		m_id = id;
	}

	const std::string& AppInfo::getName() const
	{
		return m_name;
	}

	void AppInfo::setName(const std::string& name)
	{
		m_name = name;
	}

	const std::string& AppInfo::getShortName() const
	{
		return m_shortName;
	}

	void AppInfo::setShortName(const std::string& shortName)
	{
		m_shortName = shortName;
	}

	const std::string& AppInfo::getEnglishShortName() const
	{
		return m_englishShortName;
	}

	void AppInfo::setEnglishShortName(const std::string& englishShortName)
	{
		m_englishShortName = englishShortName;
	}

	const std::string& AppInfo::getDescription() const
	{
		return m_description;
	}

	void AppInfo::setDescription(const std::string& description)
	{
		m_description = description;
	}

	const Version& AppInfo::getVersion() const
	{
		return m_version;
	}

	void AppInfo::setVersion(const Version& version)
	{
		m_version = version;
	}

	const std::string& AppInfo::getChangelog() const
	{
		return m_changelog;
	}

	void AppInfo::setChangelog(const std::string& changelog)
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
		std::unique_ptr<maddy::Parser> parser{ std::make_unique<maddy::Parser>() };
		m_htmlChangelog = parser->Parse(markdown);
	}

	const std::string& AppInfo::getHtmlChangelog() const
	{
		return m_htmlChangelog;
	}

	const std::string& AppInfo::getSourceRepo() const
	{
		return m_sourceRepo;
	}

	void AppInfo::setSourceRepo(const std::string& sourceRepo)
	{
		m_sourceRepo = sourceRepo;
	}

	const std::string& AppInfo::getIssueTracker() const
	{
		return m_issueTracker;
	}

	void AppInfo::setIssueTracker(const std::string& issueTracker)
	{
		m_issueTracker = issueTracker;
	}

	const std::string& AppInfo::getSupportUrl() const
	{
		return m_supportUrl;
	}

	void AppInfo::setSupportUrl(const std::string& supportUrl)
	{
		m_supportUrl = supportUrl;
	}

	std::map<std::string, std::string>& AppInfo::getExtraLinks()
	{
		return m_extraLinks;
	}

	const std::map<std::string, std::string>& AppInfo::getExtraLinks() const
	{
		return m_extraLinks;
	}

	std::map<std::string, std::string>& AppInfo::getDevelopers()
	{
		return m_developers;
	}

	const std::map<std::string, std::string>& AppInfo::getDevelopers() const
	{
		return m_developers;
	}

	std::map<std::string, std::string>& AppInfo::getDesigners()
	{
		return m_designers;
	}

	const std::map<std::string, std::string>& AppInfo::getDesigners() const
	{
		return m_designers;
	}

	std::map<std::string, std::string>& AppInfo::getArtists()
	{
		return m_artists;
	}

	const std::map<std::string, std::string>& AppInfo::getArtists() const
	{
		return m_artists;
	}

	const std::string& AppInfo::getTranslatorCredits() const
	{
		return m_translatorCredits;
	}

	void AppInfo::setTranslatorCredits(const std::string& translatorCredits)
	{
		m_translatorCredits = translatorCredits;
	}

	std::vector<std::string> AppInfo::getTranslatorNames() const
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

	std::vector<std::string> AppInfo::convertUrlMapToVector(const std::map<std::string, std::string>& urls)
	{
		std::vector<std::string> vec;
		for (const std::pair<const std::string, std::string>& pair : urls)
		{
			vec.push_back(pair.first + " " + pair.second);
		}
		return vec;
	}
}