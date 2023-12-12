#ifndef VERSION_H
#define VERSION_H

#include <iostream>
#include <string>
#include "versiontype.h"

namespace Nickvision::Aura
{
	/**
	 * @brief A model for a version number. Formatted in "major.minor.build-dev".
	 */
	class Version
	{
	public:
		/**
		 * @brief Constructs a Version. 
		 */
		Version();
		/**
		 * @brief Constructs a Version.
		 * @param major The major number
		 * @param minor The minor number
		 * @param build The build number
		 */
		Version(int major, int minor, int build);
		/**
		 * @brief Constructs a Version.
		 * @param major The major number
		 * @param minor The minor number
		 * @param build The build number
		 * @param dev The dev string
		 */
		Version(int major, int minor, int build, const std::string& dev);
		/**
		 * @brief Constructs a Version.
		 * @param version A version string to parse
		 */
		Version(const std::string& version);
		/**
		 * @brief Gets the type of the version.
		 * @return VersionType
		 */
		VersionType getVersionType() const;
		/**
		 * @brief Gets a string representation of the Version.
		 * @return The string representation of the Version
		 */
		const std::string& toString() const;
		/**
		 * @brief Gets whether or not the Version object is empty
		 * @return True if empty, else false
		 */
		bool empty() const;
		/**
		 * @brief Compares Version objects via < operator
		 * @param compare The Version object to compare too
		 * @return True if this < compare
		 */
		bool operator<(const Version& compare) const;
		/**
		 * @brief Compares Version objects via <= operator
		 * @param compare The Version object to compare too
		 * @return True if this <= compare
		 */
		bool operator<=(const Version& compare) const;
		/**
		 * @brief Compares Version objects via > operator
		 * @param compare The Version object to compare too
		 * @return True if this > compare
		 */
		bool operator>(const Version& compare) const;
		/**
		 * @brief Compares Version objects via >= operator
		 * @param compare The Version object to compare too
		 * @return True if this >= compare
		 */
		bool operator>=(const Version& compare) const;
		/**
		 * @brief Compares Version objects via == operator
		 * @param compare The Version object to compare too
		 * @return True if this == compare
		 */
		bool operator==(const Version& compare) const;
		/**
		 * @brief Compares Version objects via != operator
		 * @param compare The Version object to compare too
		 * @return True if this != compare
		 */
		bool operator!=(const Version& compare) const;
		/**
		 * @brief Outputs the Version object 
		 */
		friend std::ostream& operator<<(std::ostream& os, const Version& version);

	private:
		int m_major;
		int m_minor;
		int m_build;
		std::string m_dev;
		std::string m_str;
	};
}

#endif // VERSION_H