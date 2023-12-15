#include "filesystem/filesystemwatcher.h"
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#endif

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
		//Initialization
#ifdef _WIN32
		HANDLE folder{ CreateFileW(m_path.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, nullptr) };
		if (folder == INVALID_HANDLE_VALUE)
		{
			return;
		}
		DWORD bytes{ 0 };
		OVERLAPPED overlapped{ 0 };
		overlapped.hEvent = CreateEventA(nullptr, true, false, nullptr);
		if (!overlapped.hEvent)
		{
			CloseHandle(folder);
			return;
		}
		std::vector<BYTE> buffer(1024 * 256);
#endif
		//Watch
		while (m_watching)
		{
#ifdef _WIN32
			ReadDirectoryChangesW(folder, buffer.data(), static_cast<DWORD>(buffer.size()), true, (DWORD)m_watcherFlags, &bytes, &overlapped, nullptr);
			if (WaitForSingleObject(overlapped.hEvent, INFINITE) == WAIT_OBJECT_0)
			{
				if (!GetOverlappedResult(folder, &overlapped, &bytes, true))
				{
					break;
				}
				if (bytes != 0)
				{
					continue;
				}
				FILE_NOTIFY_INFORMATION* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&buffer[0]);
				while (true)
				{
					std::filesystem::path changed{ std::wstring(info->FileName, info->FileNameLength / sizeof(info->FileName[0])) };
					if (info->Action == FILE_ACTION_ADDED)
					{
						m_changed.invoke({ changed, FileAction::Added });
					}
					else if (info->Action == FILE_ACTION_REMOVED)
					{
						m_changed.invoke({ changed, FileAction::Removed });
					}
					else if (info->Action == FILE_ACTION_MODIFIED)
					{
						m_changed.invoke({ changed, FileAction::Modified });
					}
					else if(info->Action == FILE_ACTION_RENAMED_OLD_NAME)
					{
						m_changed.invoke({ changed, FileAction::Renamed });
					}
					info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<BYTE*>(info) + info->NextEntryOffset);
				}
			}
			else
			{
				break;
			}
#endif
		}
		//Cleanup
#ifdef _WIN32
		CloseHandle(folder);
#endif
	}
}