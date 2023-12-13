#ifndef FILESYSTEMCHANGEDEVENTARGS_H
#define FILESYSTEMCHANGEDEVENTARGS_H

#include <filesystem>
#include "watcherflags.h"
#include "events/eventargs.h"

namespace Nickvision::Aura::Filesystem
{
	/**
	 * @brief Event args for when a file system object is changed.
	 */
	class FileSystemChangedEventArgs : public Events::EventArgs
	{
	public:
		/**
		 * @brief Constructs a FileSystemChangedEventArgs.
		 * @param path The path of the file/folder that changed
		 * @param why The flag that was changed on the file/folder
		 */
		FileSystemChangedEventArgs(const std::filesystem::path& path, WatcherFlags why);
		/**
		 * @brief Gets the path of the changed file system object.
		 * @return The path of the changed file/folder
		 */
		const std::filesystem::path& getPath() const;
		/**
		 * @brief Gets the flag that was changed on the file system object.
		 * @return The flah that was changed on the file/folder
		 */
		WatcherFlags getWhy() const;

	private:
		std::filesystem::path m_path;
		WatcherFlags m_why;
	};
}

#endif //FILESYSTEMCHANGEDEVENTARGS_H