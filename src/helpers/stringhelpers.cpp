#include "helpers/stringhelpers.h"
#include <algorithm>
#include <array>
#include <codecvt>
#include <iomanip>
#include <limits>
#include <locale>
#include <regex>
#include <sstream>
#include <curl/curl.h>
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <uuid/uuid.h>
#endif

namespace Nickvision
{
	std::string StringHelpers::toLower(std::string s) noexcept
	{
		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
		return s;
	}

	std::string StringHelpers::toUpper(std::string s) noexcept
	{
		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
		return s;
	}

	std::string StringHelpers::trim(const std::string& s) noexcept
	{
		std::string result{ s };
		result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) 
		{
			return !std::isspace(ch);
		}).base(), result.end());
		result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch)
		{
			return !std::isspace(ch);
		}));
		return result;
	}

	std::string StringHelpers::trim(const std::string& s, char delimiter) noexcept
	{
		std::string result{ s };
		result.erase(std::find_if(result.rbegin(), result.rend(), [delimiter](char ch)
			{
				return ch != delimiter;
			}).base(), result.end());
		result.erase(result.begin(), std::find_if(result.begin(), result.end(), [delimiter](char ch)
			{
				return ch != delimiter;
			}));
		return result;
	}

	std::string StringHelpers::replace(std::string s, const std::string& toReplace, const std::string& replace) noexcept
	{
		if (s.empty() || toReplace.empty())
		{
			return s;
		}
		size_t r{ s.find(toReplace) };
		if (r == std::string::npos)
		{
			return s;
		}
		return s.replace(r, toReplace.size(), replace);
	}

	unsigned int StringHelpers::stoui(const std::string& s, size_t* idx, int base) noexcept
	{
		try
		{
			unsigned long ul{ std::stoul(s, idx, base) };
			if (ul > (std::numeric_limits<unsigned>::max)())
			{
				return (std::numeric_limits<unsigned>::max)();
			}
			return static_cast<unsigned int>(ul);
		}
		catch (...)
		{
			return 0;
		}
	}

	std::string StringHelpers::newGuid() noexcept
	{
		std::array<unsigned char, 16> guid;
#ifdef _WIN32
		GUID win;
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (CoCreateGuid(&win) != S_OK)
		{
			return "";
		}
		guid = {
			(unsigned char)((win.Data1 >> 24) & 0xFF),
			(unsigned char)((win.Data1 >> 16) & 0xFF),
			(unsigned char)((win.Data1 >> 8) & 0xFF),
			(unsigned char)((win.Data1) & 0xff),

			(unsigned char)((win.Data2 >> 8) & 0xFF),
			(unsigned char)((win.Data2) & 0xff),

			(unsigned char)((win.Data3 >> 8) & 0xFF),
			(unsigned char)((win.Data3) & 0xFF),

			(unsigned char)win.Data4[0],
			(unsigned char)win.Data4[1],
			(unsigned char)win.Data4[2],
			(unsigned char)win.Data4[3],
			(unsigned char)win.Data4[4],
			(unsigned char)win.Data4[5],
			(unsigned char)win.Data4[6],
			(unsigned char)win.Data4[7]
		};
#elif defined(__linux__)
		uuid_generate(guid.data());
#endif
		std::ostringstream out;
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[0]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[1]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[2]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[3]);
		out << "-";
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[4]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[5]);
		out << "-";
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[6]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[7]);
		out << "-";
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[8]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[9]);
		out << "-";
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[10]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[11]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[12]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[13]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[14]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[15]);
		return out.str();
	}

	bool StringHelpers::isValidUrl(const std::string& s) noexcept
	{
		if (s.empty())
		{
			return false;
		}
		CURLU* url{ curl_url() };
		int res{ curl_url_set(url, CURLUPART_URL, s.c_str(), 0) };
		curl_url_cleanup(url);
		return res == CURLUE_OK;
	}

	std::string StringHelpers::toString(const std::wstring& s) noexcept
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(s);
	}

	std::wstring StringHelpers::toWstring(const std::string& s) noexcept
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(s);
	}

	std::string StringHelpers::join(const std::vector<std::string>& values, const std::string& separator, bool separateLast) noexcept
	{
		std::stringstream builder;
		for(size_t i = 0; i < values.size(); i++)
		{
			const std::string& value{ values[i] };
			builder << value;
			if(i != values.size() - 1 || (i == values.size() - 1 && separateLast))
			{
				builder << separator;
			}
		}
		return builder.str();
	}
}