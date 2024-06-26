#ifndef APPINFO_H
#define APPINFO_H

#include <string>
#include <unordered_map>
#include <vector>
#include "update/version.h"

namespace Nickvision::App
{
    /**
     * @brief A model for the information about an application.
     */
    class AppInfo
    {
    public:
        /**
         * @brief Constructs an AppInfo.
         */
        AppInfo() = default;
        /**
         * @brief Constructs an AppInfo.
         * @param id The application id
         * @param name The application name
         * @param englishShortName The application short name in English
         */
        AppInfo(const std::string& id, const std::string& name, const std::string& englishShortName);
        /**
         * @brief Gets the application id.
         * @return The application id
         */
        const std::string& getId() const;
        /**
         * @brief Sets the application id.
         * @param id The application id
         */
        void setId(const std::string& id);
        /**
         * @brief Gets the application name.
         * @return The application name
         */
        const std::string& getName() const;
        /**
         * @brief Sets the application name.
         * @param name The application name
         */
        void setName(const std::string& name);
        /**
         * @brief Gets the application short name.
         * @return The application short name
         */
        const std::string& getShortName() const;
        /**
         * @brief Sets the application short name.
         * @param shortName The application short name
         */
        void setShortName(const std::string& shortName);
        /**
         * @brief Gets the application English short name.
         * @return The application English short name
         */
        const std::string& getEnglishShortName() const;
        /**
         * @brief Sets the application English short name.
         * @param englishShortName The application English short name
         */
        void setEnglishShortName(const std::string& englishShortName);
        /**
         * @brief Gets the application description.
         * @return The application description
         */
        const std::string& getDescription() const;
        /**
         * @brief Sets the application description.
         * @param description The application description
         */
        void setDescription(const std::string& description);
        /**
         * @brief Gets the application version.
         * @return The application version
         */
        const Update::Version& getVersion() const;
        /**
         * @brief Sets the application version.
         * @param version The application version
         */
        void setVersion(const Update::Version& version);
        /**
         * @brief Gets the application changelog.
         * @return The application changelog
         */
        const std::string& getChangelog() const;
        /**
         * @brief Sets the application changelog.
         * @param changelog The application changelog
         */
        void setChangelog(const std::string& changelog);
        /**
         * @brief Gets the application changelog in HTML form
         * @return The application changelog in HTML
         */
        const std::string& getHtmlChangelog() const;
        /**
         * @brief Gets the application source repo url
         * @return The application source repo url
         */
        const std::string& getSourceRepo() const;
        /**
         * @brief Sets the application source repo url
         * @param sourceRepo The application source repo url
         * @return True if the new source repo was set, else false
         * @return A false return value means that the sourceRepo param was not a valid formatted URL
         */
        bool setSourceRepo(const std::string& sourceRepo);
        /**
         * @brief Gets the application issue tracker url
         * @return The application issue tracker url
         */
        const std::string& getIssueTracker() const;
        /**
         * @brief Sets the application issue tracker url
         * @param issueTracker The application issue tracker url
         * @return True if the new issue tracker was set, else false
         * @return A false return value means that the issueTracker param was not a valid formatted URL
         */
        bool setIssueTracker(const std::string& issueTracker);
        /**
         * @brief Gets the application support url
         * @return The application support url
         */
        const std::string& getSupportUrl() const;
        /**
         * @brief Sets the application support url
         * @param supportUrl The application support url
         * @return True if the new support url was set, else false
         * @return A false return value means that the supportUrl param was not a valid formatted URL
         */
        bool setSupportUrl(const std::string& supportUrl);
        /**
         * @brief Gets the application html docs store url
         * @return The application html docs store url
         */
        const std::string& getHtmlDocsStore() const;
        /**
         * @brief Sets the application html docs store url
         * @param supportUrl The application html docs store url
         */
        void setHtmlDocsStore(const std::string& htmlDocsStore);
        /**
         * @brief Gets the extra links for the application
         * @return The extra links for the application
         */
        std::unordered_map<std::string, std::string>& getExtraLinks();
        /**
         * @brief Gets the extra links for the application
         * @return The extra links for the application
         */
        const std::unordered_map<std::string, std::string>& getExtraLinks() const;
        /**
         * @brief Gets the application developers
         * @return The application developers
         */
        std::unordered_map<std::string, std::string>& getDevelopers();
        /**
         * @brief Gets the application developers
         * @return The application developers
         */
        const std::unordered_map<std::string, std::string>& getDevelopers() const;
        /**
         * @brief Gets the application designers
         * @return The application designers
         */
        std::unordered_map<std::string, std::string>& getDesigners();
        /**
         * @brief Gets the application designers
         * @return The application designers
         */
        const std::unordered_map<std::string, std::string>& getDesigners() const;
        /**
         * @brief Gets the application artists
         * @return The application artists
         */
        std::unordered_map<std::string, std::string>& getArtists();
        /**
         * @brief Gets the application artists
         * @return The application artists
         */
        const std::unordered_map<std::string, std::string>& getArtists() const;
        /**
         * @brief Gets the application translator credits
         * @return The application translator credits
         */
        const std::string& getTranslatorCredits() const;
        /**
         * @brief Sets the application translator credits
         * @param translatorCredits The application translator credits
         */
        void setTranslatorCredits(const std::string& translatorCredits);
        /**
         * @brief Gets the list of translator names
         * @return The application translator credits
         */
        std::vector<std::string> getTranslatorNames() const;

    private:
        std::string m_id;
        std::string m_name;
        std::string m_shortName;
        std::string m_englishShortName;
        std::string m_description;
        Update::Version m_version;
        std::string m_changelog;
        std::string m_htmlChangelog;
        std::string m_sourceRepo;
        std::string m_issueTracker;
        std::string m_supportUrl;
        std::string m_htmlDocsStore;
        std::unordered_map<std::string, std::string> m_extraLinks;
        std::unordered_map<std::string, std::string> m_developers;
        std::unordered_map<std::string, std::string> m_designers;
        std::unordered_map<std::string, std::string> m_artists;
        std::string m_translatorCredits;

    public:
        /**
         * @brief Converts a map of URLs to a vector
         * @param urls The map of URLs
         * @return The vector of URLs
         */
        static std::vector<std::string> convertUrlMapToVector(const std::unordered_map<std::string, std::string>& urls);
    };
}

#endif // APPINFO_H
