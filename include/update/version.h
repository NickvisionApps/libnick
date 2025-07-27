/**
 * @file
 * @author Nicholas Logozzo <nlogozzo225@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * A model for a version number.
 */

#ifndef VERSION_H
#define VERSION_H

#include <iostream>
#include <string>
#include "versiontype.h"

namespace Nickvision::Update
{
    /**
     * @brief A model for a version number.
     * @brief Formatted in "major.minor.build-dev".
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
         * @throw std::invalid_argument Thrown when the dev version does not contain a "-" or a "." as the first character
         */
        Version(int major, int minor, int build, const std::string& dev);
        /**
         * @brief Constructs a Version.
         * @param version A version string to parse
         * @throw std::invalid_argument Thrown when the version string is not formatted correctly
         */
        Version(const std::string& version);
        /**
         * @brief Gets the major number of the version.
         * @return The major number
         */
        int getMajor() const;
        /**
         * @brief Gets the minor number of the version.
         * @return The minor number
         */
        int getMinor() const;
        /**
         * @brief Gets the build number of the version.
         * @return The build number
         */
        int getBuild() const;
        /**
         * @brief Gets the dev string of the version.
         * @return The dev string
         */
        const std::string& getDev() const;
        /**
         * @brief Gets the type of the version.
         * @return VersionType
         */
        VersionType getVersionType() const;
        /**
         * @brief Gets a string representation of the Version.
         * @return The string representation of the Version
         */
        const std::string& str() const;
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