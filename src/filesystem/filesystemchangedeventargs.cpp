#include "filesystem/filesystemchangedeventargs.h"

namespace Nickvision::Aura::Filesystem
{
	FileSystemChangedEventArgs::FileSystemChangedEventArgs(const std::filesystem::path& path, WatcherFlags why)
		: m_path{ path },
		m_why{ why }
	{

	}

	const std::filesystem::path& FileSystemChangedEventArgs::getPath() const
	{
		return m_path;
	}

	WatcherFlags FileSystemChangedEventArgs::getWhy() const
	{
		return m_why;
	}
}