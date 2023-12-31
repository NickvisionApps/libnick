#include "filesystem/filesystemchangedeventargs.h"

namespace Nickvision::Aura::Filesystem
{
	FileSystemChangedEventArgs::FileSystemChangedEventArgs(const std::filesystem::path& path, FileAction why) noexcept
		: m_path{ path },
		m_why{ why }
	{

	}

	const std::filesystem::path& FileSystemChangedEventArgs::getPath() const noexcept
	{
		return m_path;
	}

	FileAction FileSystemChangedEventArgs::getWhy() const noexcept
	{
		return m_why;
	}
}