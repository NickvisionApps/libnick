#include "dependencylocator.h"
#include <map>
#include "systemdirectories.h"

namespace Nickvision::Aura
{
	const std::optional<std::filesystem::path>& DependencyLocator::find(std::string dependency)
	{
		static std::map<std::string, std::optional<std::filesystem::path>> locations;
#ifdef _WIN32
		if (!std::filesystem::path(dependency).has_extension())
		{
			dependency += ".exe";
		}
#endif
		if (locations.contains(dependency))
		{
			const std::optional<std::filesystem::path>& location = locations[dependency];
			if (location && std::filesystem::exists(location.value()))
			{
				return location;
			}
		}
		locations[dependency] = std::nullopt;
		std::filesystem::path current{ std::filesystem::current_path() / dependency };
		if (std::filesystem::exists(current))
		{
			locations[dependency] = current;
		}
		else
		{
			for (const std::filesystem::path& dir : SystemDirectories::getPath())
			{
				std::filesystem::path path{ dir / dependency };
				if (std::filesystem::exists(path) && dir.string().find("AppData\\Local\\Microsoft\\WindowsApps") == std::string::npos)
				{
					locations[dependency] = path;
					break;
				}
			}
		}
		return locations[dependency];
	}
}