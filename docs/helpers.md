# Nickvision::Helpers

This module contains various helper namespaces that making working with C++ a bit less painful.

## Table of Contents
- [CodeHelpers](#codehelpers)
- [StringHelpers](#stringhelpers)

## CodeHelpers
Description: Helper functions for working with C++

Interface: [codehelpers.h](/include/helpers/codehelpers.h)

Type: `namespace`

Path: `Nickvision::CodeHelpers`

### Functions
- ```cpp
  const T& unmove(T&& t)
  ```
    - Accepts: An rvalue, t
    - Returns: The lvalue of t.

## StringHelpers
Description: Helper functions for working with std::string

Interface: [stringhelpers.h](/include/helpers/stringhelpers.h)

Type: `namespace`

Path: `Nickvision::StringHelpers`

### Functions
- ```cpp
  std::vector<std::uint8_t> decode(const std::string& base64)
  ```
    - Accepts: A base64 encoded string
    - Returns: The list of bytes from the base64 encoded string.
    - Returns: An empty list on error.
- ```cpp
  std::string encode(const std::vector<std::uint8_t>& bytes)
  ```
    - Accepts: A list of bytes (char)
    - Returns: The base64 encoded string of the bytes list.
- ```cpp
  bool isValidUrl(const std::string& s)
  ```
    - Accepts: A string parameter, s.
    - Returns: `true` if s is a valid formatted URL.
    - Returns: `false` if s is not formatted correctly. 
    - Note: If this function returns `true`, it does not mean that the URL points to a valid, live website. Use `WebHelpers::isValidWebsite()` for that check.
    - Ex: `StringHelpers::isValidUrl("https://www.google.com")` will return `true`.
- ```cpp
  std::string join(const std::vector<std::string>& values, const std::string& separator, bool separateLast = true)
  ```
    - Accepts: The list of strings to join, values, the string to insert as a separator between each value, separator, and whether or not to include the separator for the last element, separateLast.
    - Returns: A single string that consists of all elements of values separated by delimiter.
    - Ex: `StringHelpers::join({ "hi", "bye" }, "|")` will return `"hi|bye"`.
- ```cpp 
  std::string lower(std::string s)
  ```
    - Accepts: A string parameter, s.
    - Returns: The lowercase version of s. 
    - Ex: `StringHelpers::lower("aBC")` will return `"abc"`.
- ```cpp
  std::string newGuid()
  ```
    - Returns: A newly generated guid/uuid string.
    - Ex: `StringHelpers::newGuid()` can return `"93f464a0-8c36-4631-8c8e-a92edd4d6fe9"`.
- ```cpp
  std::string replace(std::string s, const std::string& toReplace, const std::string& replace)
  ```
    - Accepts: A string parameter, s, a substring of s to replace, toReplace, and a string to use as the replacement, replace.
    - Returns: A new string with toReplace replaced with replace.
    - Ex: `StringHelpers::replace("hello bye hi", "bye", "goodbye")` will return `"hello goodbye bye"`.
- ```cpp
  std::vector<T> split<T>(std::string s, const std::string& delimiter)
  ```
    - Accepts: A string parameter, s, and a string delimiter, delimiter.
    - Returns: A list of the splits of s on delimiter and casted to type T. 
    - Note: T requires the `StringImplicitlyConstructible` concept defined as: `std::is_constructible_v<T, std::string> && std::is_convertible_v<std::string, T>`.
    - Ex: `StringHelpers::split<std::string>("1,2,3,4", ",")` will return `std::vector<std::string>{ "1", "2", "3", "4" }`.
- ```cpp
  std::string str(const std::wstring& s)
  ```
    - Accepts: A wide string parameter, s.
    - Returns: s as a non-wide string.
    - Ex: `StringHelpers::str(L"abc")` will return `"abc"`.
- ```cpp
  unsigned int stoui(const std::string& s, size_t* idx = nullptr, int base = 10)
  ```
    - Accepts: A string parameter, s, a pointer to size_t, idx, and a number base, base.
    - Returns: The conversion of s to an unsigned integer with the number base base. and stores the number of characters parsed in idx.
    - Returns: UINT_MAX if s as an unsigned int exceeds the bounds of an unsigned int.
    - Out: If idx != nullptr, the number of characters parsed will be stored in idx.
    - Ex: `StringHelpers::stoui("2837914")` will return `2837914`.
- ```cpp
  std::string trim(const std::string& s)
  ```
    - Accepts: A string parameter, s.
    - Returns: A new string with removed whitespace from the beginning and the end of s. 
    - Ex: `StringHelpers::trim("     hello  ")` will return `"hello"`.
- ```cpp
  std::string trim(const std::string& s, char delimiter)
  ```
    - Accepts: A string parameter, s, and a character delimiter, delimiter. 
    - Returns: A new string with delimiter removed from the beginning and the end of s.
    - Ex: `StringHelpers::trim("---bye-", '-')` will return `"bye"`.
- ```cpp
  std::string upper(std::string s)
  ```
    - Accepts: A string parameter, s.
    - Returns: The uppercase version of s. 
    - Ex: `StringHelpers::upper("uoP")` will return `"UOP"`.
- ```cpp
  std::wstring wstr(const std::string& s)
  ```
    - Accepts: A non-wide string parameter, s.
    - Returns: s as a wide string.
    - Ex: `StringHelpers::wstr("abc")` will return `L"abc"`.
