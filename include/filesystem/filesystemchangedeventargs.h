#ifndef FILESYSTEMCHANGEDEVENTARGS_H
#define FILESYSTEMCHANGEDEVENTARGS_H

#include <filesystem>
#include "fileaction.h"
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
		 * @param why The action that caused the file to change
		 */
		FileSystemChangedEventArgs(const std::filesystem::path& path, FileAction why);
		/**
		 * @brief Gets the path of the changed file system object.
		 * @return The path of the changed file/folder
		 */
		const std::filesystem::path& getPath() const;
		/**
		 * @brief Gets the action that caused the file to change.
		 * @return The action that caused the file to change
		 */
		FileAction getWhy() const;

	private:
		std::filesystem::path m_path;
		FileAction m_why;
	};
}

#endif //FILESYSTEMCHANGEDEVENTARGS_H