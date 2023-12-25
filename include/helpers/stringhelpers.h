#ifndef STRINGHELPERS_H
#define STRINGHELPERS_H

#include <string>
#include <vector>

namespace Nickvision::Aura::StringHelpers
{
	/**
	 * @brief Gets a fully lowercase string from the provided string.
	 * @param s The string to get lowercase
	 * @return The new lowercase string
	 */
	std::string toLower(std::string s);
	/**
	 * @brief Gets a fully uppercase string from the provided string.
	 * @param s The string to get uppercase
	 * @return The new uppercase string
	 */
	std::string toUpper(std::string s);
	/**
	 * @brief Trims whitespace form the beginning and end of a string.
	 * @param s The string to trim (unmodified)
	 * @return The new trimmed string
	 */
	std::string trim(const std::string& s);
	/**
	 * @brief Trims the delimiter character form the beginning and end of a string.
	 * @param s The string to trim (unmodified)
	 * @param delimiter The character to trim
	 * @return The new trimmed string
	 */
	std::string trim(const std::string& s, char delimiter);
	/**
	 * @brief Splits a string based on a delimiter.
	 * @tparam T The type of the resulting splits
	 * @param s The string to split
	 * @param delimiter The delimiter to split the string on
	 * @return The splits of the string
	 */
	template<typename T = std::string>
	std::vector<T> split(std::string s, const std::string& delimiter)
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
	std::string replace(std::string s, std::string toReplace, std::string replace);
	/**
	 * @brief Generates a new guid value.
	 * @return The guid value
	 */
	std::string newGuid();
	/**
	 * @brief Gets whether or not the provided string is a valid url
	 * @param s The string to check
	 * @return True if the string is a valid url, else false
	 */
	bool isValidUrl(const std::string& s);

}

#endif // STRINGHELPERS_H
