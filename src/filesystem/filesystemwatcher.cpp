#include "filesystem/filesystemwatcher.h"
#include <algorithm>

namespace Nickvision::Aura::Filesystem
{
	FileSystemWatcher::FileSystemWatcher(const std::filesystem::path& path, WatcherFlags watcherFlags)
		: m_path{ path },
		m_watching{ true },
		m_watcherFlags{ watcherFlags },
		m_watchThread{ &FileSystemWatcher::watch, this }
	{
		
	}

	FileSystemWatcher::~FileSystemWatcher()
	{
		m_watching = false;
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

	bool FileSystemWatcher::containsExtension(const std::filesystem::path& extension)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		return std::find(m_extensionFilters.begin(), m_extensionFilters.end(), extension) != m_extensionFilters.end();
	}

	bool FileSystemWatcher::addExtensionFilter(const std::filesystem::path& extension)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		if (std::find(m_extensionFilters.begin(), m_extensionFilters.end(), extension) == m_extensionFilters.end())
		{
			m_extensionFilters.push_back(extension);
			return true;
		}
		return false;
	}

	bool FileSystemWatcher::removeExtensionFilter(const std::filesystem::path& extension)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
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
		std::lock_guard<std::mutex> lock{ m_mutex };
		m_extensionFilters.clear();
		return true;
	}

	void FileSystemWatcher::watch()
	{
		while (m_watching)
		{

		}
	}
}