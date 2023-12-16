#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include <filesystem>
#include <mutex>
#include <thread>
#include <vector>
#include "filesystemchangedeventargs.h"
#include "watcherflags.h"
#include "events/event.h"

namespace Nickvision::Aura::Filesystem
{
	/**
	 * @brief A watcher of a file system folder.
	 */
	class FileSystemWatcher
	{
	public:
		/**
		 * @brief Constructs a FileSystemWatcher.
		 * @param path The path of the folder to watch
		 * @param watcherFlags The flags of what to watch changes for
		 */
		FileSystemWatcher(const std::filesystem::path& path, WatcherFlags watcherFlags = WatcherFlags::FileName | WatcherFlags::DirectoryName | WatcherFlags::Atributes | WatcherFlags::Size | WatcherFlags::LastWrite | WatcherFlags::LastAccess | WatcherFlags::CreationTime | WatcherFlags::Security);
		/**
		 * @brief Deconstructs a FileSystemWatcher. 
		 */
		~FileSystemWatcher();
		/**
		 * @brief Gets the path of the file system object being watched.
		 * @return The path of the folder being watched
		 */
		const std::filesystem::path& getPath() const;
		/**
		 * @brief Gets the event for when the watched file system object is changed.
		 * @return The changed event
		 */
		Events::Event<FileSystemChangedEventArgs>& changed();
		/**
		 * @brief Gets the flags of what to watch changed for.
		 * @return The flags of watched properties
		 */
		WatcherFlags getWatcherFlags() const;
		/**
		 * @brief Gets whether or not the file extension is being watched.
		 * @param extension The file extension to check
		 * @return True if watched, else false
		 */
		bool containsExtension(const std::filesystem::path& extension);
		/**
		 * @brief Adds an extension of a file to watch for changes in the folder.
		 * @param extension The file extension to add
		 * @return True if successful, else false
		 */
		bool addExtensionFilter(const std::filesystem::path& extension);
		/**
		 * @brief Removes an extension of a file to watch for changes in the folder.
		 * @param extension The file extension to remove
		 * @return True if successful, else false
		 */
		bool removeExtensionFilter(const std::filesystem::path& extension);
		/**
		 * @brief Clears all extensions to watch. 
		 * @return True if successful, else false
		 */
		bool clearExtensionFilters();

	private:
		/**
		 * @brief Runs the loop to watch a folder for changes.
		 */
		void watch();
		mutable std::mutex m_mutex;
		std::filesystem::path m_path;
		bool m_watching;
		Events::Event<FileSystemChangedEventArgs> m_changed;
		WatcherFlags m_watcherFlags;
		std::vector<std::filesystem::path> m_extensionFilters;
		std::jthread m_watchThread;
	};
}

#endif //FILESYSTEMWATCHER_H