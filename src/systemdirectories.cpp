#include "systemdirectories.h"
#include "aura.h"
#include "helpers/stringhelpers.h"

namespace Nickvision::Aura
{
	std::vector<std::filesystem::path> SystemDirectories::getPath()
	{
		std::string var{ Aura::getEnvVar("PATH") };
		if (!var.empty())
		{
#ifdef _WIN32
			return StringHelpers::split<std::filesystem::path>(var, ";");
#elif defined(__linux__)
			return StringHelpers::split<std::filesystem::path>(var, ":");
#endif
		}
		return {};
	}

	std::vector<std::filesystem::path> SystemDirectories::getConfig()
	{
		std::string var{ Aura::getEnvVar("XDG_CONFIG_DIRS") };
		if (!var.empty())
		{
#ifdef _WIN32
			return StringHelpers::split<std::filesystem::path>(var, ";");
#elif defined(__linux__)
			return StringHelpers::split<std::filesystem::path>(var, ":");
#endif
		}
		return {};
	}

	std::vector<std::filesystem::path> SystemDirectories::getData()
	{
		std::string var{ Aura::getEnvVar("XDG_DATA_DIRS") };
		if (!var.empty())
		{
#ifdef _WIN32
			return StringHelpers::split<std::filesystem::path>(var, ";");
#elif defined(__linux__)
			return StringHelpers::split<std::filesystem::path>(var, ":");
#endif
		}
		return {};
	}
}