# Nickvision::Localization

This module contains functions for working with translations and other localization aspects of Nickvision applications.

## Table of Contents
- [Documentation](#documentation)
- [Gettext](#gettext)

## Documentation
Description: Functions for managing credentials in the system's credential (secret) store.

Interface: [documentation.h](/include/localization/documentation.h)

Type: `namespace`

Path: `Nickvision::Localization::Documentation`

### Functions
- ```cpp
  std::string getHelpUrl(const std::string& pageName)
  ``` 
    - Accepts: The name of the documentation page to get the help url for, pageName.
    - Returns: The url for the documentation page. This will be a yelp url for Linux and a website url for Windows and Linux snaps.
    - Note: HtmlDocsStore should be set for Aura::getActive()::getAppInfo(). For Nickvision apps, this will be: `https://raw.githubusercontent.com/NickvisionApps/SHORT_APP_NAME/main/APP_NAME.Shared/Docs/html`, but can be customized for any app.

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
    - Note: This function should only be called once, regradless of with different domain names.
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