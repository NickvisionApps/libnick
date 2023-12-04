#ifndef STORE_H
#define STORE_H

#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>
#include "credential.h"

namespace Nickvision::Aura::Keyring
{
	/**
	 * @brief A store object for credentials. Backed by sqlciphe.
	 */
	class Store
	{
	public:
		/**
		 * @brief Copies a Store object.
		 * @parma store The object to move
		 */
		Store(const Store& store);
		/**
		 * @brief Moves a Store object.
		 * @parma store The object to move
		 */
		Store(Store&& store) noexcept;
		/**
		 * @brief Gets the name of the store.
		 * @return The name of the store
		 */
		const std::string& getName() const;
		/**
		 * @brief Gets the file path of the store on disk.
		 * @return The file path of the store
		 */
		const std::filesystem::path& getPath() const;
		/**
		 * @brief Gets all credentials in the store.
		 * @return The list of all credentials
		 */
		std::vector<Credential> getAllCredentials() const;
		/**
		 * @brief Gets the credential matching the provided id.
		 * @param id The id of the credential
		 * @return The credential matching the id, std::nullopt if no matching credential
		 */
		std::optional<Credential> getCredential(int id) const;
		/**
		 * @brief Gets the credentials containing the provided name.
		 * @param name The name
		 * @return The list of credentials matching the name
		 */
		std::vector<Credential> getCredentials(const std::string& name) const;
		/**
		 * @brief Adds a credential to the store.
		 * @param credential The Credential to add
		 * @return True if successful, else false
		 */
		bool addCredential(const Credential& credential);
		/**
		 * @brief Updates a credential in the store.
		 * @param credential The Credential to update
		 * @return True if successful, else false
		 */
		bool updateCredential(const Credential& credential);
		/**
		 * @brief Deletes a credential from the store.
		 * @param id The id of the credential to delete
		 * @return True if successful, else false
		 */
		bool deleteCredential(int id);
		/**
		 * @brief Destroys the store and all of its data from disk. Once this method is called, the object should no longer be referenced, with or without success.
		 * @return True if successful, else false
		 */
		bool destroy();

	private:
		Store(const std::string& name, const std::shared_ptr<SQLite::Database>& database);
		mutable std::mutex m_mutex;
		std::shared_ptr<SQLite::Database> m_database;
		std::string m_name;
		std::filesystem::path m_path;

	public:
		/**
		 * @brief Gets the directory where stores are saved on disk.
		 * @return The directory for stores
		 */
		static std::filesystem::path getStoreDir();
		/**
		 * @brief Creates a new store.
		 * @param name The name of the store
		 * @param password The password of the store
		 * @param overwrite Whether or not to overwrite a store that already exists with the provided name
		 * @return The created Store object, std::nullopt if creation failed
		 */
		static std::optional<Store> create(const std::string& name, const std::string& password, bool overwrite = true);
		/**
		 * @brief Loads a store.
		 * @param name The name of the store
		 * @param password The password of the store
		 * @return The loaded Store object, std::nullopt if loading failed
		 */
		static std::optional<Store> load(const std::string& name, const std::string& password);
		/**
		 * @brief Gets whether or not a store exists with the provided name.
		 * @param name The name of the store to check
		 * @return True if a store with the provied name exists, else false
		 */
		static bool exists(const std::string& name);
		/**
		 * @brief Destroys a store and all of its data from disk.
		 * @param The name of the store to destroy
		 * @return True if successful, else false
		 */
		static bool destroy(const std::string& name);
	};
}

#endif //STORE_H