#include "filesystem/filesystemwatcher.h"
#include <algorithm>

namespace Nickvision::Aura::Filesystem
{
	FileSystemWatcher::FileSystemWatcher(const std::filesystem::path& path, bool incudeSubdirectories, WatcherFlags watcherFlags)
		: m_path{ path },
		m_includeSubdirectories{ incudeSubdirectories },
		m_watcherFlags{ watcherFlags },
		m_watching{ true },
		m_watchThread{ &FileSystemWatcher::watch, this }
	{
		
	}

	FileSystemWatcher::~FileSystemWatcher()
	{
		m_watching = false;
#ifdef _WIN32
		if (m_terminateEvent)
		{
			SetEvent(m_terminateEvent);
			CloseHandle(m_terminateEvent);
		}
#endif
	}

	const std::filesystem::path& FileSystemWatcher::getPath() const
	{
		return m_path;
	}

	WatcherFlags FileSystemWatcher::getWatcherFlags() const
	{
		return m_watcherFlags;
	}

	bool FileSystemWatcher::getIncludeSubdirectories() const
	{
		return m_includeSubdirectories;
	}

	Events::Event<FileSystemChangedEventArgs>& FileSystemWatcher::changed()
	{
		return m_changed;
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
#ifdef _WIN32
		m_terminateEvent = CreateEventA(nullptr, 1, 0, nullptr);
		if (!m_terminateEvent)
		{
			return;
		}
		HANDLE folder{ CreateFileW(m_path.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, nullptr) };
		if (folder == INVALID_HANDLE_VALUE)
		{
			return;
		}
		OVERLAPPED overlapped{ 0 };
		overlapped.hEvent = CreateEvent(nullptr, 1, 0, nullptr);
		if (!overlapped.hEvent)
		{
			CloseHandle(folder);
			return;
		}
		std::vector<BYTE> buffer(1024 * 256);
		DWORD bytes{ 0 };
		bool pending{ false };
		HANDLE events[2]{ overlapped.hEvent, m_terminateEvent };
		while (m_watching)
		{
			pending = ReadDirectoryChangesW(folder, &buffer[0], DWORD(buffer.size()), m_includeSubdirectories ? 1 : 0, DWORD(m_watcherFlags), &bytes, &overlapped, nullptr);
			if (WaitForMultipleObjects(2, events, 0, INFINITE) == WAIT_OBJECT_0)
			{
				if (!GetOverlappedResult(folder, &overlapped, &bytes, 1) || bytes == 0)
				{
					CloseHandle(folder);
					return;
				}
				pending = false;
				FILE_NOTIFY_INFORMATION* info{ reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&buffer[0]) };
				while(true)
				{
					if (info->Action != FILE_ACTION_RENAMED_NEW_NAME)
					{
						std::filesystem::path changed{ std::wstring(info->FileName, info->FileNameLength / sizeof(info->FileName[0])) };
						if (m_extensionFilters.size() == 0 || containsExtension(changed.extension()))
						{
							m_changed.invoke({ changed , static_cast<FileAction>(info->Action) });
						}
					}
					if (info->NextEntryOffset == 0)
					{
						break;
					}
					info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<BYTE*>(info) + info->NextEntryOffset);
				}
			}
			else
			{
				break;
			}
		}
		if (pending)
		{
			CancelIo(folder);
			GetOverlappedResult(folder, &overlapped, &bytes, TRUE);
		}
		CloseHandle(folder);
#endif
	}
}