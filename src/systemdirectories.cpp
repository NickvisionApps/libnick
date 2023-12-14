#include "systemdirectories.h"
#include <cstdlib>
#include "helpers/stringhelpers.h"

namespace Nickvision::Aura
{

	std::vector<std::filesystem::path> SystemDirectories::getPath()
	{
		char* var = std::getenv("PATH");
		if (var)
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
		char* var = std::getenv("XDG_CONFIG_DIRS");
		if (var)
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
		char* var = std::getenv("XDG_DATA_DIRS");
		if (var)
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