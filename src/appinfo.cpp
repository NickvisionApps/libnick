#include "appinfo.h"
#include <memory>
#include <sstream>
#include <maddy/parser.h>
#include "helpers/stringhelpers.h"

namespace Nickvision::Aura
{
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
		std::unique_ptr<maddy::Parser> parser{ std::make_unique<maddy::Parser>() };
		m_htmlChangelog = parser->Parse(markdown);
	}

	const std::string& AppInfo::getHtmlChangelog() const noexcept
	{
		return m_htmlChangelog;
	}

	const std::string& AppInfo::getSourceRepo() const noexcept
	{
		return m_sourceRepo;
	}

	void AppInfo::setSourceRepo(const std::string& sourceRepo) noexcept
	{
		m_sourceRepo = sourceRepo;
	}

	const std::string& AppInfo::getIssueTracker() const noexcept
	{
		return m_issueTracker;
	}

	void AppInfo::setIssueTracker(const std::string& issueTracker) noexcept
	{
		m_issueTracker = issueTracker;
	}

	const std::string& AppInfo::getSupportUrl() const noexcept
	{
		return m_supportUrl;
	}

	void AppInfo::setSupportUrl(const std::string& supportUrl) noexcept
	{
		m_supportUrl = supportUrl;
	}

	const std::string& AppInfo::getHtmlDocsStore() const noexcept
	{
		return m_htmlDocsStore;
	}

	void AppInfo::setHtmlDocsStore(const std::string& htmlDocsStore) noexcept
	{
		m_htmlDocsStore = htmlDocsStore;
	}

	std::map<std::string, std::string>& AppInfo::getExtraLinks() noexcept
	{
		return m_extraLinks;
	}

	const std::map<std::string, std::string>& AppInfo::getExtraLinks() const noexcept
	{
		return m_extraLinks;
	}

	std::map<std::string, std::string>& AppInfo::getDevelopers() noexcept
	{
		return m_developers;
	}

	const std::map<std::string, std::string>& AppInfo::getDevelopers() const noexcept
	{
		return m_developers;
	}

	std::map<std::string, std::string>& AppInfo::getDesigners() noexcept
	{
		return m_designers;
	}

	const std::map<std::string, std::string>& AppInfo::getDesigners() const noexcept
	{
		return m_designers;
	}

	std::map<std::string, std::string>& AppInfo::getArtists() noexcept
	{
		return m_artists;
	}

	const std::map<std::string, std::string>& AppInfo::getArtists() const noexcept
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

	std::vector<std::string> AppInfo::convertUrlMapToVector(const std::map<std::string, std::string>& urls) noexcept
	{
		std::vector<std::string> vec;
		for (const std::pair<const std::string, std::string>& pair : urls)
		{
			vec.push_back(pair.first + " " + pair.second);
		}
		return vec;
	}
}