#include "update/version.h"
#include <stdexcept>
#include "helpers/stringhelpers.h"

using namespace Nickvision::Helpers;

namespace Nickvision::Update
{
    Version::Version() noexcept
        : m_major{ 0 },
        m_minor{ 0 },
        m_build{ 0 },
        m_str{ "0.0.0" }
    {

    }

    Version::Version(int major, int minor, int build) noexcept
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
        if (m_dev[0] != '-' && m_dev[0] != '.')
        {
            throw std::invalid_argument("Dev version must contain a '-' or '.' as the first character.");
        }
    }

    Version::Version(const std::string& version)
        : m_major{ 0 },
        m_minor{ 0 },
        m_build{ 0 }
    {

        std::vector<std::string> splits{ StringHelpers::split(version, ".", false) };
        if(splits.size() < 3)
        {
            throw std::invalid_argument("Ill-formated version string.");
        }
        m_major = std::stoi(splits[0]);
        m_minor = std::stoi(splits[1]);
        if(splits.size() == 3)
        {
            if(splits[2].find("-") == std::string::npos)
            {
                m_build = std::stoi(splits[2]);
            }
            else
            {
                std::vector<std::string> devSplits{ StringHelpers::split(splits[2], "-", false) };
                if(devSplits.size() != 2)
                {
                    throw std::invalid_argument("Ill-formated version string.");
                }
                m_build = std::stoi(devSplits[0]);
                m_dev = devSplits[1];
            }
        }
        else if(splits.size() == 4)
        {
            m_build = std::stoi(splits[2]);
            m_dev = splits[3];
        }
        else
        {
            throw std::invalid_argument("Ill-formated version string.");
        }
        m_str = std::to_string(m_major) + "." + std::to_string(m_minor) + "." + std::to_string(m_build);
        if(!m_dev.empty())
        {
            m_str += "-" + m_dev;
        }
    }

    int Version::getMajor() const noexcept
    {
        return m_major;
    }

    int Version::getMinor() const noexcept
    {
        return m_minor;
    }

    int Version::getBuild() const noexcept
    {
        return m_build;
    }

    const std::string& Version::getDev() const noexcept
    {
        return m_dev;
    }

    VersionType Version::getVersionType() const noexcept
    {
        return m_dev.empty() ? VersionType::Stable : VersionType::Preview;
    }

    const std::string& Version::str() const noexcept
    {
        return m_str;
    }

    bool Version::empty() const noexcept
    {
        return m_major == 0 && m_minor == 0 && m_build == 0 && m_dev.empty();
    }

    bool Version::operator<(const Version& compare) const noexcept
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

    bool Version::operator<=(const Version& compare) const noexcept
    {
        return operator<(compare) || operator==(compare);
    }

    bool Version::operator>(const Version& compare) const noexcept
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

    bool Version::operator>=(const Version& compare) const noexcept
    {
        return operator>(compare) || operator==(compare);
    }

    bool Version::operator==(const Version& compare) const noexcept
    {
        return m_major == compare.m_major && m_minor == compare.m_minor && m_build == compare.m_build && m_dev == compare.m_dev;
    }

    bool Version::operator!=(const Version& compare) const noexcept
    {
        return !(operator==(compare));
    }
}
