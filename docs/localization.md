# Nickvision::Localization

This module contains functions for working with translations and other localization aspects of Nickvision applications.

## Table of Contents
- [Documentation](#documentation)
- [Gettext](#gettext)

## Documentation
Description: Functions for working with an application's documentation.

Interface: [documentation.h](/include/localization/documentation.h)

Type: `namespace`

Path: `Nickvision::Localization::Documentation`

### Methods
- ```cpp
  std::string getHelpUrl(const std::string& englishShortName, const std::string& htmlDocStore, const std::string& pageName)
  ```
    - Accepts: The english short name of the application, englishShortName, the html docs store url, htmlDocStore, and the name of the documentation page to get, pageName.
    - Returns: The url for pageName.

## Gettext
Description: Functions for managing credentials in the system's credential (secret) store.

Interface: [gettext.h](/include/localization/gettext.h)

Type: `namespace`

Path: `Nickvision::Localization::Gettext`

### Macros
- Note: All strings passed to these macros must be C-Style Strings (`const char*`).
- ```cpp
  #define _(String) gettext(String)
  ``` 
    - Accepts: A string to translate, String.
    - Returns: The translated message.
- ```cpp
  #define _n(String, StringPlural, N) ngettext(String, StringPlural, N)
  ``` 
    - Accepts: A singular version of string to translate, String, a plural version of string to translate, StringPlural, and the number of objects, N, used to determine whether or not to use the plural version of the message.
    - Returns: The translated message for the given number of objects.
- ```cpp
  #define _p(Context, String) ::Nickvision::Localization::pgettext(Context GETTEXT_CONTEXT_SEPARATOR String, String)
  ``` 
   - Accepts: The context of a string to translate, context, and the string to translate, msg.
   - Returns: The translated message for the given context.
- ```cpp
  #define _pn(Context, String, StringPlural, N) ::Nickvision::Localization::pngettext(Context GETTEXT_CONTEXT_SEPARATOR String, String, StringPlural, N)
  ```
   - Accepts: The context of a string to translate, context, the singular version of string to translate, msg, the plural version of the string to translate, and the number of objects, n, used to determine whether or not to use the plural version of the message.
   - Returns: The translated message for the given context and number of objects.

### Functions
- ```cpp
  bool init(const std::string& domainName)
  ``` 
    - Accepts: The domain name for the gettext system, domainName.
    - Returns: `true` if the gettext system was initialized successfully.
    - Returns: `false` if there was an error initializing the gettext system.
    - Note: This function should only be called once, regardless of with different domain names.
    - Note: domainName will be modified to ensure it is lowercase and contains no spaces.
- ```cpp
  const std::string& getDomainName();
  ```
    - Returns: The domain name used for gettext translations.
- ```cpp
  const char* pgettext(const char* context, const char* msg)
  ```
   - Accepts: The context of a string to translate, context, and the string to translate, msg.
   - Returns: The translated message for the given context.
- ```cpp
  const char* pngettext(const char* context, const char* msg, const char* msgPlural, unsigned long n)
  ```
   - Accepts: The context of a string to translate, context, the singular version of string to translate, msg, the plural version of the string to translate, and the number of objects, n, used to determine whether or not to use the plural version of the message.
   - Returns: The translated message for the given context and number of objects.