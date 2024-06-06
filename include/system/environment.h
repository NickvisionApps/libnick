#if (defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS))
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>

namespace Nickvision::System::Environment
{
    /**
     * @brief Gets the value of an environment variable.
     * @param key The environment variable to get
     * @return The environment variable value if found, else empty string
     */
    std::string getVariable(const std::string& key);
    /**
     * @brief Sets the value of an environment variable.
     * @param key The environment variable to set
     * @param value The value for the environment variable
     * @return True if set, else false
     */
    bool setVariable(const std::string& key, const std::string& value);
    /**
     * @brief Clears an environment variable.
     * @param key The environment variable to delete
     * @return True if deleted, else false
     */
    bool clearVariable(const std::string& key);
    /**
     * @brief Executes a command in the system shell.
     * @param cmd The command to execute
     * @return The output of the command
     */
    std::string exec(const std::string& cmd);
}

#endif //ENVIRONMENT_H