#include "helpers/stringhelpers.h"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <cwchar>
#include <locale>
#include <limits>
#include <regex>
#include <sstream>
#include "system/environment.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <gio/gio.h>
#endif

using namespace Nickvision::System;

namespace Nickvision::Helpers
{
    std::vector<std::byte> StringHelpers::decode(const std::string& base64)
    {
        if(base64.empty() || base64.size() % 4 != 0)
        {
            return {};
        }
        static const unsigned char lookup[128]{ 
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  62, 255,  62, 255,  63,
        52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255, 255, 255, 255, 255,
        255,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
        15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255,  63,
        255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
        41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 255, 255, 255, 255, 255 };
        std::vector<std::byte> bytes;
        bytes.reserve(3 * base64.size() / 4);
        for(size_t i = 0; i < base64.size(); i += 4)
        {
            unsigned char b641{ base64[i] <= 122 ? lookup[static_cast<unsigned char>(base64[i])] : static_cast<unsigned char>(0xff) };
            unsigned char b642{ base64[i + 1] <= 122 ? lookup[static_cast<unsigned char>(base64[i + 1])] : static_cast<unsigned char>(0xff) };
            unsigned char b643{ base64[i + 2] <= 122 ? lookup[static_cast<unsigned char>(base64[i + 2])] : static_cast<unsigned char>(0xff) };
            unsigned char b644{ base64[i + 3] <= 122 ? lookup[static_cast<unsigned char>(base64[i + 3])] : static_cast<unsigned char>(0xff) };
            std::byte b1{ static_cast<unsigned char>(((b641 & 0x3f) << 2) + ((b642 & 0x30) >> 4)) };
            std::byte b2{ static_cast<unsigned char>(((b642 & 0x0f) << 4) + ((b643 & 0x3c) >> 2)) };
            std::byte b3{ static_cast<unsigned char>(((b643 & 0x03) << 6) + (b644 & 0x3f)) };
            if(b642 != 0xff)
            {
                bytes.push_back(b1);
            }
            if(b643 != 0xff)
            {
                bytes.push_back(b2);
            }
            if(b644 != 0xff)
            {
                bytes.push_back(b3);
            }
        }
        return bytes;
    }

    std::string StringHelpers::encode(const std::vector<std::byte>& bytes)
    {
        if(bytes.empty())
        {
            return "";
        }
        static const char lookup[65]{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };
        size_t missing{ 0 };
        size_t stringSize{ bytes.size() };
        while((stringSize % 3) != 0)
        {
            stringSize++;
            missing++;
        }
        stringSize = 4 * stringSize / 3;
        std::string string;
        string.reserve(stringSize);
        for(size_t i = 0; i < stringSize / 4; i++)
        {
            size_t index{ i * 3 };
            unsigned char b1{ index < bytes.size() ? static_cast<unsigned char>(bytes[index]) : static_cast<unsigned char>(0) };
            unsigned char b2{ index + 1 < bytes.size() ? static_cast<unsigned char>(bytes[index + 1]) : static_cast<unsigned char>(0) };
            unsigned char b3{ index + 2 < bytes.size() ? static_cast<unsigned char>(bytes[index + 2]) : static_cast<unsigned char>(0) };
            unsigned char b641{ static_cast<unsigned char>((b1 & 0xfc) >> 2) };
            unsigned char b642{ static_cast<unsigned char>(((b1 & 0x03) << 4) + ((b2 & 0xf0) >> 4)) };
            unsigned char b643{ static_cast<unsigned char>(((b2 & 0x0f) << 2) + ((b3 & 0xc0) >> 6)) };
            unsigned char b644{ static_cast<unsigned char>(b3 & 0x3f) };
            string.push_back(lookup[b641]);
            string.push_back(lookup[b642]);
            string.push_back(lookup[b643]);
            string.push_back(lookup[b644]);
        }
        for(size_t i = 0; i != missing; i++)
        {
            string[stringSize - i - 1] = '=';
        }
        return string;
    }

    bool StringHelpers::isValidUrl(const std::string& s)
    {
        if (s.empty())
        {
            return false;
        }
        static std::regex urlRegex{ R"(^(ht|f)tp(s?)\:\/\/[0-9a-zA-Z]([-.\w]*[0-9a-zA-Z])*(:(0-9)*)*(\/?)([a-zA-Z0-9\-\.\?\,\'\/\\\+&amp;%\$#_]*)?$)" };
        return std::regex_match(s, urlRegex);
    }

    std::string StringHelpers::join(const std::vector<std::string>& values, const std::string& separator, bool separateLast)
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

    std::string StringHelpers::newUuid()
    {
#ifdef _WIN32
        GUID guid;
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (CoCreateGuid(&guid) != S_OK)
        {
            return "";
        }
        std::array<wchar_t, 40> buffer;
        if(StringFromGUID2(guid, buffer.data(), 40) == 0)
        {
            return "";
        }
        return StringHelpers::str({ buffer.data() }).substr(1, 36);
#else
        return g_uuid_string_random();
#endif
    }

    std::string StringHelpers::newGuid()
    {
        return newUuid();
    }

    std::string StringHelpers::normalizeForFilename(const std::string& s, bool windowsOnly)
    {
        std::string result{ s };
        result = replace(result, '/', '_'); //invalid on all operating systems
        if(windowsOnly || Environment::getOperatingSystem() == OperatingSystem::Windows)
        {
            result = replace(result, '<', '_');
            result = replace(result, '>', '_');
            result = replace(result, ':', '_');
            result = replace(result, '"', '_');
            result = replace(result, '\\', '_');
            result = replace(result, '|', '_');
            result = replace(result, '?', '_');
            result = replace(result, '*', '_');
        }
        return result;
    }

    std::string StringHelpers::replace(std::string s, const std::string& toReplace, const std::string& replace)
    {
        if (s.empty() || toReplace.empty())
        {
            return s;
        }
        size_t pos{ 0 };
        while((pos = s.find(toReplace, pos)) != std::string::npos)
        {
            s.replace(pos, toReplace.size(), replace);
            pos += replace.size();
        }
        return s;
    }

    std::string StringHelpers::replace(std::string s, char toReplace, char replace)
    {
        if (s.empty())
        {
            return s;
        }
        std::replace(s.begin(), s.end(), toReplace, replace);
        return s;
    }

    std::vector<std::string> StringHelpers::splitArgs(std::string s)
    {
        std::vector<std::string> args;
        std::regex regex{ "((?:[^\\s'\"]+|\"[^\"]*\"|'[^']*')+)" };
        std::smatch match;
        while (std::regex_search(s, match, regex))
        {
            std::string arg{ match.str() };
            if((arg[0] == '\'' && arg[arg.size() - 1] == '\'') || (arg[0] == '"' && arg[arg.size() - 1] == '"'))
            {
                arg = arg.substr(1, arg.size() - 2);
            }
            args.push_back(arg);
            s = match.suffix();
        }
        if(!s.empty())
        {
            args.push_back(s);
        }
        return args;
    }

    std::string StringHelpers::str(const std::wstring& s)
    {
#ifdef _WIN32
        int size{ WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, nullptr, 0, nullptr, nullptr) };
        if(size <= 0)
        {
            return {};
        }
        std::string res(size - 1, 0);
        WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, &res[0], size, nullptr, nullptr);
        return res;
#else
        std::mbstate_t state{};
        const wchar_t* ptr{ s.data() };
        size_t size{ 1 + std::wcsrtombs(nullptr, &ptr, 0, &state) };
        if(size == static_cast<std::size_t>(-1))
        {
            return {};
        }
        std::vector<char> buf(size);
        std::wcsrtombs(buf.data(), &ptr, buf.size(), &state);
        return std::string(buf.data());
#endif
    }

    unsigned int StringHelpers::stoui(const std::string& s, size_t* idx, int base)
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

    std::string StringHelpers::lower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
        return s;
    }

    std::string StringHelpers::upper(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
        return s;
    }

    std::string StringHelpers::trim(const std::string& s)
    {
        if(s.empty())
        {
            return s;
        }
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

    std::string StringHelpers::trim(const std::string& s, char delimiter)
    {
        if(s.empty())
        {
            return s;
        }
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

    std::wstring StringHelpers::wstr(const std::string& s)
    {
#ifdef _WIN32
        int size{ MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0) };
        if(size <= 0)
        {
            return {};
        }
        std::wstring res(size - 1, 0);
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &res[0], size);
        return res;
#else
        std::mbstate_t state{};
        const char* ptr{ s.data() };
        size_t size{ 1 + std::mbsrtowcs(nullptr, &ptr, 0, &state) };
        if(size == static_cast<std::size_t>(-1))
        {
            return {};
        }
        std::vector<wchar_t> buf(size);
        std::mbsrtowcs(buf.data(), &ptr, buf.size(), &state);
        return std::wstring(buf.data());
#endif
    }
}
