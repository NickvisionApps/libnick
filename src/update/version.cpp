#include "update/version.h"
#include <stdexcept>
#include <iostream>

namespace Nickvision::Update
{
    Version::Version()
        : m_major{ 0 },
        m_minor{ 0 },
        m_build{ 0 },
        m_str{ "0.0.0" }
    {

    }

    Version::Version(int major, int minor, int build)
        : m_major{ major },
        m_minor{ minor },
        m_build{ build },
        m_str{ std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(build) }
    {

    }

    Version::Version(int major, int minor, int build, const std::string& dev)
        : m_major{ major },
        m_minor{ minor },
        m_build{ build },
        m_dev{ dev },
        m_str{ std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(build) + dev }
    {
        if (m_dev[0] != '-')
        {
            throw std::invalid_argument("Dev version must contain a '-'.");
        }
    }

    Version::Version(const std::string& version)
        : m_major{ 0 },
        m_minor{ 0 },
        m_build{ 0 }
    {
        std::string s{ version };
        size_t pos{ 0 };
        int i{ 0 };
        while ((pos = s.find('.')) != std::string::npos)
        {
            std::string token{ s.substr(0, pos) };
            if (i == 0) 
            {
                m_major = std::stoi(token);
            }
            else if (i == 1)
            {
                m_minor = std::stoi(token);
            }
            else
            {
                throw std::invalid_argument("Ill-formated version string.");
            }
            s.erase(0, pos + 1);
            i++;
        }
        if (i != 2)
        {
            throw std::invalid_argument("Ill-formated version string.");
        }
        size_t dashIndex{ s.find('-') };
        m_build = std::stoi(s.substr(0, dashIndex));
        s.erase(0, dashIndex);
        if (!s.empty() && s[0] == '-') //dev version
        {
            m_dev = s;
        }
        m_str = std::to_string(m_major) + "." + std::to_string(m_minor) + "." + std::to_string(m_build) + m_dev;
    }

    int Version::getMajor() const
    {
        return m_major;
    }

    int Version::getMinor() const
    {
        return m_minor;
    }

    int Version::getBuild() const
    {
        return m_build;
    }

    const std::string& Version::getDev() const
    {
        return m_dev;
    }

    VersionType Version::getVersionType() const
    {
        return m_dev.empty() ? VersionType::Stable : VersionType::Preview;
    }

    const std::string& Version::str() const
    {
        return m_str;
    }

    bool Version::empty() const
    {
        return m_major == 0 && m_minor == 0 && m_build == 0 && m_dev.empty();
    }

    bool Version::operator<(const Version& compare) const
    {
        if(m_major > compare.m_major)
        {
            return false;
        }
        else if(m_major < compare.m_major)
        {
            return true;
        }
        else
        {
            if(m_minor > compare.m_minor)
            {
                return false;
            }
            else if(m_minor < compare.m_minor)
            {
                return true;
            }
            else
            {
                if(m_build > compare.m_build)
                {
                    return false;
                }
                else if(m_build < compare.m_build)
                {
                    return true;
                }
                else
                {
                    if(m_dev.empty() && !compare.m_dev.empty())
                    {
                        return false;
                    }
                    else if(!m_dev.empty() && compare.m_dev.empty())
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool Version::operator<=(const Version& compare) const
    {
        return operator<(compare) || operator==(compare);
    }

    bool Version::operator>(const Version& compare) const
    {
        if(m_major > compare.m_major)
        {
            return true;
        }
        else if(m_major < compare.m_major)
        {
            return false;
        }
        else
        {
            if(m_minor > compare.m_minor)
            {
                return true;
            }
            else if(m_minor < compare.m_minor)
            {
                return false;
            }
            else
            {
                if(m_build > compare.m_build)
                {
                    return true;
                }
                else if(m_build < compare.m_build)
                {
                    return false;
                }
                else
                {
                    if(m_dev.empty() && !compare.m_dev.empty())
                    {
                        return true;
                    }
                    else if(!m_dev.empty() && compare.m_dev.empty())
                    {
                        return false;
                    }
                }
            }
        }
        return false;
    }

    bool Version::operator>=(const Version& compare) const
    {
        return operator>(compare) || operator==(compare);
    }

    bool Version::operator==(const Version& compare) const
    {
        return m_major == compare.m_major && m_minor == compare.m_minor && m_build == compare.m_build && m_dev == compare.m_dev;
    }

    bool Version::operator!=(const Version& compare) const
    {
        return !(operator==(compare));
    }

    std::ostream& operator<<(std::ostream& os, const Version& version)
    {
        os << version.str();
        return os;
    }
}