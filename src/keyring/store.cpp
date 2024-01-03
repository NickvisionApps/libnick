#include "keyring/store.h"
#include <stdexcept>
#include "userdirectories.h"

namespace Nickvision::Aura::Keyring
{
	static std::string sqlite3_column_string(sqlite3_stmt* statement, int index) noexcept
	{
		sqlite3_column_bytes(statement, index);
		const char* data{ static_cast<const char*>(sqlite3_column_blob(statement, index)) };
		return { data, static_cast<size_t>(sqlite3_column_bytes(statement, index)) };
	}

	static std::filesystem::path getPathFromName(const std::string& name) noexcept
	{
		std::filesystem::path dir{ UserDirectories::getConfig() / "Nickvision" / "Keyring" };
		std::filesystem::create_directories(dir);
		return dir / (name + ".ring");
	}

	Store::Store(const std::string& name, const std::string& password)
		: m_name{ name },
		m_password{ password },
		m_database{ nullptr },
		m_path{ getPathFromName(name) }
	{
		sqlite3* database{ nullptr };
		char* err{ nullptr };
		if (sqlite3_open_v2(m_path.string().c_str(), &database, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) == SQLITE_OK)
		{
			if (sqlite3_key(database, m_password.c_str(), static_cast<int>(m_password.size())) == SQLITE_OK)
			{
				if (sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS credentials (id TEXT PRIMARY KEY, name TEXT, uri TEXT, username TEXT, password TEXT)", nullptr, nullptr, &err) == SQLITE_OK)
				{
					m_database = { database, [](sqlite3* sql)
					{
						sqlite3_close(sql);
					}};
					return;
				}
				else
				{
					std::string error{ err ? err : "" };
					sqlite3_free(err);
					sqlite3_close(database);
					throw std::runtime_error("Unable to exec create table command. Key may be invalid. " + error);
				}
			}
			else
			{
				sqlite3_close(database);
				throw std::runtime_error("Unable to key the database.");
			}
		}
		else
		{
			throw std::runtime_error("Unable to open the database.");
		}
	}

	Store::Store(const Store& store) noexcept
	{
		std::lock_guard<std::mutex> lock{ store.m_mutex };
		m_name = store.m_name;
		m_password = store.m_password;
		m_database = store.m_database;
		m_path = store.m_path;
	}

	Store::Store(Store&& store) noexcept
	{
		std::lock_guard<std::mutex> lock{ store.m_mutex };
		m_name = std::move(store.m_name);
		m_password = std::move(store.m_password);
		m_database = std::move(store.m_database);
		m_path = std::move(store.m_path);
	}

	const std::string& Store::getName() const noexcept
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		return m_name;
	}

	const std::filesystem::path& Store::getPath() const noexcept
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		return m_path;
	}

	std::vector<Credential> Store::getAllCredentials() const noexcept
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		std::vector<Credential> creds;
		if (m_database)
		{
			sqlite3_stmt* statement;
			if (sqlite3_prepare_v2(m_database.get(), "SELECT * FROM credentials", -1, &statement, nullptr) == SQLITE_OK)
			{
				while (sqlite3_step(statement) == SQLITE_ROW)
				{
					creds.push_back({ sqlite3_column_int(statement, 0), sqlite3_column_string(statement, 1), sqlite3_column_string(statement, 2), sqlite3_column_string(statement, 3), sqlite3_column_string(statement, 4) });
				}
				sqlite3_finalize(statement);
			}
		}
		return creds;
	}

	std::optional<Credential> Store::getCredential(int id) const noexcept
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		std::optional<Credential> cred{ std::nullopt };
		if (m_database)
		{
			sqlite3_stmt* statement;
			if (sqlite3_prepare_v2(m_database.get(), "SELECT * FROM credentials where id = ?", -1, &statement, nullptr) == SQLITE_OK)
			{
				if (sqlite3_bind_int(statement, 1, id) == SQLITE_OK)
				{
					if (sqlite3_step(statement) == SQLITE_ROW)
					{
						cred = { { sqlite3_column_int(statement, 0), sqlite3_column_string(statement, 1), sqlite3_column_string(statement, 2), sqlite3_column_string(statement, 3), sqlite3_column_string(statement, 4) } };
					}
				}
				sqlite3_finalize(statement);
			}
		}
		return cred;
	}

	std::vector<Credential> Store::getCredentials(const std::string& name) const noexcept
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		std::vector<Credential> creds;
		if (m_database)
		{
			sqlite3_stmt* statement;
			if (sqlite3_prepare_v2(m_database.get(), "SELECT * FROM credentials where name = ?", -1, &statement, nullptr) == SQLITE_OK)
			{
				if (sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
				{
					while (sqlite3_step(statement) == SQLITE_ROW)
					{
						creds.push_back({ sqlite3_column_int(statement, 0), sqlite3_column_string(statement, 1), sqlite3_column_string(statement, 2), sqlite3_column_string(statement, 3), sqlite3_column_string(statement, 4) });
					}
				}
				sqlite3_finalize(statement);
			}
		}
		return creds;
	}

	bool Store::addCredential(const Credential& credential) noexcept
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		bool res{ false };
		if (m_database)
		{
			sqlite3_stmt* statement;
			if (sqlite3_prepare_v2(m_database.get(), "INSERT INTO credentials (id, name, uri, username, password) VALUES (?,?,?,?,?)", -1, &statement, nullptr) == SQLITE_OK)
			{
				if (sqlite3_bind_int(statement, 1, credential.getId()) == SQLITE_OK)
				{
					if (sqlite3_bind_text(statement, 2, credential.getName().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
					{
						if (sqlite3_bind_text(statement, 3, credential.getUri().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
						{
							if (sqlite3_bind_text(statement, 4, credential.getUsername().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
							{
								if (sqlite3_bind_text(statement, 5, credential.getPassword().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
								{
									res = sqlite3_step(statement) == SQLITE_DONE;
								}
							}
						}
					}
				}
				sqlite3_finalize(statement);
			}
		}
		return res;
	}

	bool Store::updateCredential(const Credential& credential) noexcept
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		bool res{ false };
		if (m_database)
		{
			sqlite3_stmt* statement;
			if (sqlite3_prepare_v2(m_database.get(), "UPDATE credentials SET name = ?, uri = ?, username = ?, password = ? where id = ?", -1, &statement, nullptr) == SQLITE_OK)
			{
				if (sqlite3_bind_int(statement, 5, credential.getId()) == SQLITE_OK)
				{
					if (sqlite3_bind_text(statement, 1, credential.getName().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
					{
						if (sqlite3_bind_text(statement, 2, credential.getUri().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
						{
							if (sqlite3_bind_text(statement, 3, credential.getUsername().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
							{
								if (sqlite3_bind_text(statement, 4, credential.getPassword().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
								{
									res = sqlite3_step(statement) == SQLITE_DONE;
								}
							}
						}
					}
				}
				sqlite3_finalize(statement);
			}
		}
		return res;
	}

	bool Store::deleteCredential(int id) noexcept
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		bool res{ false };
		if (m_database)
		{
			sqlite3_stmt* statement;
			if (sqlite3_prepare_v2(m_database.get(), "DELETE FROM credentials WHERE id = ?", -1, &statement, nullptr) == SQLITE_OK)
			{
				if (sqlite3_bind_int(statement, 1, id) == SQLITE_OK)
				{
					res = sqlite3_step(statement) == SQLITE_DONE;
				}
				sqlite3_finalize(statement);
			}
		}
		return res;
	}

	bool Store::destroy() noexcept
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		if (m_database)
		{
			sqlite3_close(m_database.get());
			m_database = nullptr;
		}
		return std::filesystem::exists(m_path) ? std::filesystem::remove(m_path) : true;
	}

	Store& Store::operator=(const Store& store) noexcept
	{
		if (this != &store)
		{
			std::lock_guard<std::mutex> lock{ m_mutex };
			std::lock_guard<std::mutex> lock2{ store.m_mutex };
			m_name = store.m_name;
			m_password = store.m_password;
			m_database = store.m_database;
			m_path = store.m_path;
		}
		return *this;
	}

	Store& Store::operator=(Store&& store) noexcept
	{
		if (this != &store)
		{
			std::lock_guard<std::mutex> lock{ m_mutex };
			std::lock_guard<std::mutex> lock2{ store.m_mutex };
			m_name = std::move(store.m_name);
			m_password = std::move(store.m_password);
			m_database = std::move(store.m_database);
			m_path = std::move(store.m_path);
		}
		return *this;
	}

	bool Store::exists(const std::string& name) noexcept
	{
		return std::filesystem::exists(getPathFromName(name));
	}

	bool Store::destroy(const std::string& name) noexcept
	{
		std::filesystem::path path{ getPathFromName(name) };
		if (std::filesystem::exists(path))
		{
			return std::filesystem::remove(path);
		}
		return true;
	}
}