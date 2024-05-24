# Nickvision::Logging

The module contains objects for logging application messages.

## Table of Contents
- [Logger](#logger)
- [LogLevel](#loglevel)

## Logger
Description: A model of an object that logs application messages.

Interface: [logger.h](/include/logging/logger.h)

Type: `class`

Path: `Nickvision::Logging::Logger`

### Member Variables
- ```
  const std::filesystem::path& Path: get
  ```
    - The path to the logs file, if available

### Methods
- ```cpp
    Logger(LogLevel minimum);
  ```
    - Constructs a Logger.
    - Accepts: The minimum log level of messages to accept, minimum.
    - Note: This object will log everything to standard outputs.
- ```cpp
    Logger(const std::filesystem::path& path, LogLevel minimum);
  ```
    - Constructs a Logger.
    - Accepts: The path of the file to write logs to, path, and the minimum log level of messages to accept, minimum.
    - Note: This object will log everything to standard outputs AND the provided file.
- ```cpp
  bool log(LogLevel level, const std::string& message, const std::source_location& source) const
  ```
	- Accepts: The level of the log message, level, the message itself, message, and the location of where this method was called from, source.

## LogLevel
Description: Levels of log messages.

Interface: [loglevel.h](/include/logging/loglevel.h)

Type: `enum class`

Path: `Nickvision::Logging::LogLevel`

### Values
- ```cpp
  Debug = 0
  ```
    - Represents debug messages.
- ```cpp
  Info = 1
  ```
    - Represents informational messages.
- ```cpp
  Warning = 2
  ```
    - Represents warning messages.
- ```cpp
  Error = 3
  ```
    - Represents error messages.
- ```cpp
  Critical = 4
  ```
    - Represents critical messages.