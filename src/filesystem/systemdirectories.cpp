#include "filesystem/systemdirectories.h"
#include "aura/aura.h"
#include "helpers/stringhelpers.h"

#ifdef _WIN32
#define ENV_VAR_PATH_SEPARATOR ";"
#elif defined(__linux__)
#define ENV_VAR_PATH_SEPARATOR ":"
#endif

namespace Nickvision::Filesystem
{
	std::vector<std::filesystem::path> SystemDirectories::getPath() noexcept
	{
		std::string var{ Aura::Aura::getEnvVar("PATH") };
		if (!var.empty())
		{
			return StringHelpers::split<std::filesystem::path>(var, ENV_VAR_PATH_SEPARATOR);
		}
		return {};
	}

	std::vector<std::filesystem::path> SystemDirectories::getConfig() noexcept
	{
		std::string var{ Aura::Aura::getEnvVar("XDG_CONFIG_DIRS") };
		if (!var.empty())
		{
			return StringHelpers::split<std::filesystem::path>(var, ENV_VAR_PATH_SEPARATOR);
		}
		return {};
	}

	std::vector<std::filesystem::path> SystemDirectories::getData() noexcept
	{
		std::string var{ Aura::Aura::getEnvVar("XDG_DATA_DIRS") };
		if (!var.empty())
		{
			return StringHelpers::split<std::filesystem::path>(var, ENV_VAR_PATH_SEPARATOR);
		}
		return {};
	}
}