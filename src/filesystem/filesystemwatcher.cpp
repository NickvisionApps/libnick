#include "filesystem/filesystemwatcher.h"
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#ifdef __linux__
#include <unistd.h>
#include <sys/inotify.h>
#endif

namespace Nickvision::Filesystem
{
    FileSystemWatcher::FileSystemWatcher(const std::filesystem::path& path, bool incudeSubdirectories, WatcherFlags watcherFlags)
        : m_path{ path },
        m_includeSubdirectories{ incudeSubdirectories },
        m_watcherFlags{ watcherFlags },
        m_watching{ true }
    {
#ifdef _WIN32
        m_terminateEvent = CreateEventW(nullptr, 1, 0, nullptr);
        if (!m_terminateEvent)
        {
            throw std::runtime_error("Unable to create event.");
        }
#elif defined(__linux__)
        m_notify = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
        if (m_notify == -1)
        {
            throw std::runtime_error("Unable to init inotify.");
        }
#elif defined(__APPLE__)
        std::string pathString{ path.string() };
        FSEventStreamContext context{ 0, this, nullptr, nullptr, nullptr };
        CFStringRef paths[1];
        paths[0] = CFStringCreateWithCString(NULL, pathString.c_str(), kCFStringEncodingUTF8);
        m_stream = FSEventStreamCreate(nullptr, &callback, &context, CFArrayCreate(nullptr, reinterpret_cast<const void**>(paths), 1, nullptr), kFSEventStreamEventIdSinceNow, 1.0, kFSEventStreamCreateFlagFileEvents);
        if (!m_stream)
        {
            throw std::runtime_error("Unable to create event stream.");
        }
        FSEventStreamScheduleWithRunLoop(m_stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
        FSEventStreamStart(m_stream);
#endif
#ifndef __APPLE__
        m_watchThread = std::thread(&FileSystemWatcher::watch, this);
#endif
    }

    FileSystemWatcher::~FileSystemWatcher()
    {
        m_watching = false;
#ifdef _WIN32
        SetEvent(m_terminateEvent);
        CloseHandle(m_terminateEvent);
#elif defined(__linux__)
        close(m_notify);
#elif defined(__APPLE__)
        FSEventStreamStop(m_stream);
        FSEventStreamInvalidate(m_stream);
        FSEventStreamRelease(m_stream);
#endif
#ifndef __APPLE__
        if(m_watchThread.joinable())
        {
            m_watchThread.join();
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

    bool FileSystemWatcher::isExtensionWatched(const std::filesystem::path& extension)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if (m_extensionFilters.size() == 0)
        {
            return true;
        }
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

#ifndef __APPLE__
    void FileSystemWatcher::watch()
    {
#ifdef _WIN32
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
                while (true)
                {
                    if (info->Action != FILE_ACTION_RENAMED_NEW_NAME)
                    {
                        std::filesystem::path changed{ std::wstring(info->FileName, info->FileNameLength / sizeof(info->FileName[0])) };
                        if (isExtensionWatched(changed.extension()))
                        {
                            m_changed({ changed , static_cast<FileAction>(info->Action) });
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
#elif defined(__linux__)
        int mask{ 0 };
        if ((m_watcherFlags & WatcherFlags::FileName) == WatcherFlags::FileName)
        {
            mask |= IN_CREATE;
            mask |= IN_DELETE;
            mask |= IN_MOVED_FROM;
        }
        if ((m_watcherFlags & WatcherFlags::DirectoryName) == WatcherFlags::DirectoryName)
        {
            mask |= IN_DELETE_SELF;
            mask |= IN_MOVE_SELF;
        }
        if ((m_watcherFlags & WatcherFlags::Attributes) == WatcherFlags::Attributes)
        {
            mask |= IN_ATTRIB;
        }
        if ((m_watcherFlags & WatcherFlags::Size) == WatcherFlags::Size)
        {
            mask |= IN_MODIFY;
        }
        if ((m_watcherFlags & WatcherFlags::LastWrite) == WatcherFlags::LastWrite)
        {
            mask |= IN_CLOSE_WRITE;
        }
        if ((m_watcherFlags & WatcherFlags::LastAccess) == WatcherFlags::LastAccess)
        {
            mask |= IN_ACCESS;
            mask |= IN_OPEN;
        }
        std::vector<int> watches;
        watches.push_back(inotify_add_watch(m_notify, m_path.c_str(), mask));
        if (m_includeSubdirectories)
        {
            for (const std::filesystem::directory_entry& e : std::filesystem::recursive_directory_iterator(m_path))
            {
                if (e.is_directory())
                {
                    watches.push_back(inotify_add_watch(m_notify, e.path().string().c_str(), mask));
                }
            }
        }
        while (m_watching)
        {
            std::vector<char> buf(1024 * (sizeof(struct inotify_event) + 16));
            ssize_t length{ read(m_notify, &buf[0], buf.size()) };
            if (length < 0)
            {
                continue;
            }
            struct inotify_event* event{ nullptr };
            for (ssize_t i = 0; i < length; i += sizeof(struct inotify_event) + event->len)
            {
                event = reinterpret_cast<struct inotify_event*>(&buf[i]);
                if (event->len)
                {
                    std::filesystem::path changed{ m_path / event->name };
                    if (isExtensionWatched(changed.extension()))
                    {
                        if (event->mask & IN_CREATE)
                        {
                            m_changed({ changed , FileAction::Added });
                        }
                        else if ((event->mask & IN_DELETE) || (event->mask & IN_DELETE_SELF))
                        {
                            m_changed({ changed , FileAction::Removed });
                        }
                        else if ((event->mask & IN_MOVED_FROM) || (event->mask & IN_MOVE_SELF))
                        {
                            m_changed({ changed , FileAction::Renamed });
                        }
                        else
                        {
                            m_changed({ changed , FileAction::Modified });
                        }
                    }
                }
            }
        }
        for (int watch : watches)
        {
            inotify_rm_watch(m_notify, watch);
        }
#endif
    }
#else
    void FileSystemWatcher::callback(ConstFSEventStreamRef stream, void* clientCallBackInfo, size_t numEvents, void* eventPaths, const FSEventStreamEventFlags eventFlags[], const FSEventStreamEventId eventIds[])
    {
        FileSystemWatcher* watcher{ static_cast<FileSystemWatcher*>(clientCallBackInfo) };
        char** paths{ static_cast<char**>(eventPaths) };
        for (size_t i = 0; i < numEvents; i++)
        {
            std::filesystem::path changed{ paths[i] };
            if (watcher->isExtensionWatched(changed.extension()))
            {
                if (eventFlags[i] & kFSEventStreamEventFlagItemCreated)
                {
                    watcher->m_changed({ changed , FileAction::Added });
                }
                else if (eventFlags[i] & kFSEventStreamEventFlagItemRemoved)
                {
                    watcher->m_changed({ changed , FileAction::Removed });
                }
                else if (eventFlags[i] & kFSEventStreamEventFlagItemRenamed)
                {
                    watcher->m_changed({ changed , FileAction::Renamed });
                }
                else
                {
                    watcher->m_changed({ changed , FileAction::Modified });
                }
            }
        }
    }
#endif
}