#ifndef STRINGHELPERS_H
#define STRINGHELPERS_H

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <string>
#include <vector>

namespace Nickvision::Aura::StringHelpers
{
	/**
	 * @brief Gets a fully lowercase string from the provided string.
	 * @param s The string to get lowercase
	 * @return The new lowercase string
	 */
	std::string toLower(std::string s) noexcept;
	/**
	 * @brief Gets a fully uppercase string from the provided string.
	 * @param s The string to get uppercase
	 * @return The new uppercase string
	 */
	std::string toUpper(std::string s) noexcept;
	/**
	 * @brief Trims whitespace form the beginning and end of a string.
	 * @param s The string to trim (unmodified)
	 * @return The new trimmed string
	 */
	std::string trim(const std::string& s) noexcept;
	/**
	 * @brief Trims the delimiter character form the beginning and end of a string.
	 * @param s The string to trim (unmodified)
	 * @param delimiter The character to trim
	 * @return The new trimmed string
	 */
	std::string trim(const std::string& s, char delimiter) noexcept;
	/**
	 * @brief Splits a string based on a delimiter.
	 * @tparam T The type of the resulting splits
	 * @param s The string to split
	 * @param delimiter The delimiter to split the string on
	 * @return The splits of the string
	 */
	template<typename T = std::string>
	std::vector<T> split(std::string s, const std::string& delimiter) noexcept
	{
		std::vector<T> splits;
		size_t pos;
		while ((pos = s.find(delimiter)) != std::string::npos)
		{
			std::string split = s.substr(0, pos);
			splits.push_back(split);
			s.erase(0, pos + 1);
		}
		if (!s.empty())
		{
			splits.push_back(s);
		}
		return splits;
	}
	/**
	 * @brief Replaces a substring within a string with a new string.
	 * @param s The string to work on
	 * @param toRepalce The substring to be replaced
	 * @param replace The new string to replace with
	 * @return The new replaced string
	 */
	std::string replace(std::string s, const std::string& toReplace, const std::string& replace) noexcept;
	/**
	 * @brief Converts a string to an unsigned int
	 * @param s The string to convert
	 * @param idx The address of a size_t to store the number of characters processed
	 * @param base The number base
	 * @return The unsigned int version of the string
	 * @return UINT_MAX if the converted string is too long
	 * @return 0 if error
	 */
	unsigned int stoui(const std::string& s, size_t* idx = nullptr, int base = 10) noexcept;
	/**
	 * @brief Generates a new guid value.
	 * @return The guid value
	 */
	std::string newGuid() noexcept;
	/**
	 * @brief Gets whether or not the provided string is a valid url
	 * @param s The string to check
	 * @return True if the string is a valid url, else false
	 */
	bool isValidUrl(const std::string& s) noexcept;
	/**
	 * @brief Converts the wstring to a string.
	 * @param s The wstring to convert
	 * @return The string version of the wstring
	 */
	std::string toString(const std::wstring& s) noexcept;
	/**
	 * @brief Converts the string to a wstring.
	 * @param s The string to convert
	 * @return The wstring version of the string
	 */
	std::wstring toWstring(const std::string& s) noexcept;

}

#endif // STRINGHELPERS_H
