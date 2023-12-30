#include "dependencylocator.h"
#include <map>
#include <mutex>
#include "systemdirectories.h"

namespace Nickvision::Aura
{
	const std::filesystem::path& DependencyLocator::find(std::string dependency) noexcept
	{
		static std::map<std::string, std::filesystem::path> locations;
		static std::mutex mutex;
		std::lock_guard<std::mutex> lock{ mutex };
#ifdef _WIN32
		if (!std::filesystem::path(dependency).has_extension())
		{
			dependency += ".exe";
		}
#endif
		if (locations.contains(dependency))
		{
			const std::filesystem::path& location = locations[dependency];
			if (std::filesystem::exists(location))
			{
				return location;
			}
		}
		locations[dependency] = std::filesystem::path();
		std::filesystem::path path{ std::filesystem::current_path() / dependency };
		if (std::filesystem::exists(path))
		{
			locations[dependency] = path;
		}
		else
		{
			for (const std::filesystem::path& dir : SystemDirectories::getPath())
			{
				path = { dir / dependency };
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