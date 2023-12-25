#include "filesystem/filesystemchangedeventargs.h"

namespace Nickvision::Aura::Filesystem
{
	FileSystemChangedEventArgs::FileSystemChangedEventArgs(const std::filesystem::path& path, FileAction why)
		: m_path{ path },
		m_why{ why }
	{

	}

	const std::filesystem::path& FileSystemChangedEventArgs::getPath() const
	{
		return m_path;
	}

	FileAction FileSystemChangedEventArgs::getWhy() const
	{
		return m_why;
	}
}