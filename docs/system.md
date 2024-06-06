# Nickvision::System

This module contains various helper namespaces and objects for working with the system shell.

## Table of Contents
- [Environment](#environment)

## Environment
Description: Helper functions for working with the system shell environment.

Interface: [environment.h](/include/system/environment.h)

Type: `namespace`

Path: `Nickvision::System::Environment`

### Methods
- ```cpp
  std::string getVariable(const std::string& key)
  ```
    - Accepts: The name of the environment variable, key.
    - Returns: The value of the environment variable with name key.
    - Returns: An empty string if no environment variable with the name key was found.
    - Ex: `Environment::getVariable("PATH")` will return the value of the system's path variable.
- ```cpp
  bool setVariable(const std::string& key, const std::string& value)
  ```
    - Accepts: The name of the environment variable, key, and the value to set said variable to, value.
    - Returns: `true` if the environment variable of name key was set to value.
    - Returns: `false` if setting the environment variable failed.
    - Ex: `Environment::setVariable("AURA", "true")` will set `"$AURA:true"`.
- ```cpp
  bool clearVariable(const std::string& key)
  ```
    - Accepts: The name of the environment variable to clear, key.
    - Returns: `true` if the environment variable of name key was cleared.
    - Returns: `false` if clearing the environment variable failed.
- ```cpp
  std::string exec(const std::string& command)
  ```
    - Accepts: The command to executed, command.
    - Returns: The output of the executed command.
    - Ex: `Environment::exec("echo Hello World")` will return `"Hello World"`.