#ifndef DEPENDENCYLOCATOR_H
#define DEPENDENCYLOCATOR_H

#include <filesystem>
#include <string>

namespace Nickvision::Aura::DependencyLocator
{
	/**
	 * @brief Finds the path of a given dependency.
	 * @param dependency The name of the dependency to find
	 * @return The path of the dependency if found, else empty path
	 */
	const std::filesystem::path& find(std::string dependency) noexcept;
}

#endif //DEPENDENCYLOCATOR_H