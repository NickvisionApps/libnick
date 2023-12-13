#include "filesystem/filesystemwatcher.h"
#include <algorithm>

namespace Nickvision::Aura::Filesystem
{
	FileSystemWatcher::FileSystemWatcher(const std::filesystem::path& path, WatcherFlags watcherFlags)
		: m_path{ path },
		m_watcherFlags{ watcherFlags }
	{

	}

	const std::filesystem::path& FileSystemWatcher::getPath() const
	{
		return m_path;
	}

	Events::Event<FileSystemChangedEventArgs>& FileSystemWatcher::changed()
	{
		return m_changed;
	}

	WatcherFlags FileSystemWatcher::getWatcherFlags() const
	{
		return m_watcherFlags;
	}

	bool FileSystemWatcher::addExtensionFilter(const std::filesystem::path& extension)
	{
		if (std::find(m_extensionFilters.begin(), m_extensionFilters.end(), extension) == m_extensionFilters.end())
		{
			m_extensionFilters.push_back(extension);
			return true;
		}
		return false;
	}

	bool FileSystemWatcher::removeExtensionFilter(const std::filesystem::path& extension)
	{
		auto find{ std::find(m_extensionFilters.begin(), m_extensionFilters.end(), extension) };
		if (find != m_extensionFilters.end())
		{
			m_extensionFilters.erase(find);
			return true;
		}
		return false;
	}

	bool FileSystemWatcher::clearExtensionFilters()
	{
		m_extensionFilters.clear();
		return true;
	}
}