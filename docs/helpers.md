# Nickvision::Helpers

This module contains various helper namespaces that making working with C++ a bit less painful.

## Table of Contents
- [StringHelpers](#stringhelpers)
- [WebHelpers](#webhelpers)

## StringHelpers
Description: Helper functions for working with std::string

Interface: [stringhelpers.h](/include/helpers/stringhelpers.h)

Type: `namespace`

Path: `Nickvision::StringHelpers`

### Functions
- ```cpp 
  std::string toLower(std::string s)
  ```
    - Accepts: A string parameter, s.
    - Returns: The lowercase version of s. 
    - Ex: `StringHelpers::toLower("aBC")` will return `"abc"`.
- ```cpp
  std::string toUpper(std::string s)
  ```
    - Accepts: A string parameter, s.
    - Returns: The uppercase version of s. 
    - Ex: `StringHelpers::toUpper("uoP")` will return `"UOP"`.
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
  std::vector<T> split<T>(std::string s, const std::string& delimiter)
  ```
    - Accepts: A string parameter, s, and a string delimiter, delimiter.
    - Returns: A list of the splits of s on delimiter and casted to type T. 
    - Note: T requires the `StringImplicitlyConstructible` concept defined as: `std::is_constructible_v<T, std::string> && std::is_convertible_v<std::string, T>`.
    - Ex: `StringHelpers::split<std::string>("1,2,3,4", ",")` will return `std::vector<std::string>{ "1", "2", "3", "4" }`.
- ```cpp
  std::string replace(std::string s, const std::string& toReplace, const std::string& replace)
  ```
    - Accepts: A string parameter, s, a substring of s to replace, toReplace, and a string to use as the replacement, replace.
    - Returns: A new string with toReplace replaced with replace.
    - Ex: `StringHelpers::replace("hello bye hi", "bye", "goodbye")` will return `"hello goodbye bye"`.
- ```cpp
  unsigned int stoui(const std::string& s, size_t* idx = nullptr, int base = 10)
  ```
    - Accepts: A string parameter, s, a pointer to size_t, idx, and a number base, base.
    - Returns: The conversion of s to an unsigned integer with the number base base. and stores the number of characters parsed in idx.
    - Returns: UINT_MAX if s as an unsigned int exceeds the bounds of an unsigned int.
    - Out: If idx != nullptr, the number of characters parsed will be stored in idx.
    - Ex: `StringHelpers::stoui("2837914")` will return `2837914`.
- ```cpp
  std::string newGuid()
  ```
    - Returns: A newly generated guid/uuid string.
    - Ex: `StringHelpers::newGuid()` can return `"93f464a0-8c36-4631-8c8e-a92edd4d6fe9"`.
- ```cpp
  bool isValidUrl(const std::string& s)
  ```
    - Accepts: A string parameter, s.
    - Returns: `true` if s is a valid formatted URL.
    - Returns: `false` if s is not formatted correctly. 
    - Note: If this function returns `true`, it does not mean that the URL points to a valid, live website. Use `WebHelpers::isValidWebsite()` for that check.
    - Ex: `StringHelpers::isValidUrl("https://www.google.com")` will return `true`.
- ```cpp
  std::string toString(const std::wstring& s)
  ```
    - Accepts: A wide string parameter, s.
    - Returns: s as a non-wide string.
    - Ex: `StringHelpers::toString(L"abc")` will return `"abc"`.
- ```cpp
  std::wstring toWstring(const std::string& s)
  ```
    - Accepts: A non-wide string parameter, s.
    - Returns: s as a wide string.
    - Ex: `StringHelpers::toWstring("abc")` will return `L"abc"`.
- ```cpp
  std::string join(const std::vector<std::string>& values, const std::string& separator, bool separateLast = true)
  ```
    - Accepts: The list of strings to join, values, the string to insert as a separator between each value, separator, and whether or not to include the separator for the last element, separateLast.
    - Returns: A single string that consists of all elements of values separated by delimiter.
    - Ex: `StringHelpers::join({ "hi", "bye" }, "|")` will return `"hi|bye"`.

## WebHelpers
Description: Helper functions for working with websites

Interface: [webhelpers.h](/include/helpers/webhelpers.h)

Type: `namespace`

Path: `Nickvision::WebHelpers`

### Functions
- ```cpp
  bool isValidWebsite(const std::string& url)
  ```
    - Accepts: A url string, url.
    - Returns: `true` if url points to a valid, live website.
    - Returns: `false` if url does not point to a valid website. 
    - Ex: `WebHelpers::isValidWebsite("https://www.sdfhjsdkfhsjdf.dfh")` will return `false`.
- ```cpp
  bool downloadFile(const std::string& url, const std::filesystem::path& path, const CurlProgressFunction& progress = {}, bool overwrite = true)
  ```
    - Accepts: A url string, url, a path of disk, path, a CurlProgressFunction, progress, and a overwrite boolean, overwrite. 
    - Returns: `true` if the file provided by url was successfully downloaded to path.
    - Returns: `false` if the download failed. 
    - Note: If progress points to a valid CurlProgressFunction, progress will be called as the file is downloaded to provide the caller with download progress. 
    - Note: If a file at path exists and overwrite is true, the file will be overwritten, else downloadFile will fail and return false.
    - Note: `CurlProgressFunction` is a `std::function` with the signature `int func(curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)`
    - Ex: `WebHelpers::downloadFile(https://raw.githubusercontent.com/nlogozzo/nlogozzo/main/README.md, "readme.md")` will return `true` and download the file to path `./readme.md`.
- ```cpp
  std::string fetchJsonString(const std::string& url)
  ```
    - Accepts: A url string, url.
    - Returns: The json string hosted at url. 
    - Note: If this function fails, an empty string will be returned.
    - Ex: `WebHelpers::fetchJsonString("https://api.github.com/repos/nickvisionapps/denaro/tags")` will return [this json string](https://api.github.com/repos/nickvisionapps/denaro/tags).