#include "keyring/store.h"
#include "userdirectories.h"

namespace Nickvision::Aura::Keyring
{
	static std::string sqlite3_column_string(sqlite3_stmt* statement, int index)
	{
		sqlite3_column_bytes(statement, index);
		const char* data{ static_cast<const char*>(sqlite3_column_blob(statement, index)) };
		return { data, static_cast<size_t>(sqlite3_column_bytes(statement, index)) };
	}

	static std::filesystem::path getPathFromName(const std::string& name)
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
		loadDatabase();
	}

	Store::Store(const Store& store)
	{
		std::lock_guard<std::mutex> lock{ store.m_mutex };
		m_name = store.m_name;
		m_password = store.m_password;
		m_database = store.m_database;
		m_path = store.m_path;
		loadDatabase();
	}

	Store::Store(Store&& store) noexcept
	{
		std::lock_guard<std::mutex> lock{ store.m_mutex };
		m_name = std::move(store.m_name);
		m_password = std::move(store.m_password);
		m_database = std::move(store.m_database);
		m_path = std::move(store.m_path);
		loadDatabase();
	}

	Store::~Store()
	{
		if (m_database)
		{
			sqlite3_close(m_database);
		}
	}

	bool Store::isValid() const
	{
		return m_database;
	}

	const std::string& Store::getName() const
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		return m_name;
	}

	const std::filesystem::path& Store::getPath() const
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		return m_path;
	}

	std::vector<Credential> Store::getAllCredentials() const
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		std::vector<Credential> creds;
		if (m_database)
		{
			sqlite3_stmt* statement;
			if (sqlite3_prepare_v2(m_database, "SELECT * FROM credentials", -1, &statement, nullptr) == SQLITE_OK)
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

	std::optional<Credential> Store::getCredential(int id) const
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		std::optional<Credential> cred{ std::nullopt };
		if (m_database)
		{
			sqlite3_stmt* statement;
			if (sqlite3_prepare_v2(m_database, "SELECT * FROM credentials where id = ?", -1, &statement, nullptr) == SQLITE_OK)
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

	std::vector<Credential> Store::getCredentials(const std::string& name) const
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		std::vector<Credential> creds;
		if (m_database)
		{
			sqlite3_stmt* statement;
			if (sqlite3_prepare_v2(m_database, "SELECT * FROM credentials where name = ?", -1, &statement, nullptr) == SQLITE_OK)
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

	bool Store::addCredential(const Credential& credential)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		bool res{ false };
		if (m_database)
		{
			sqlite3_stmt* statement;
			if (sqlite3_prepare_v2(m_database, "INSERT INTO credentials (id, name, uri, username, password) VALUES (?,?,?,?,?)", -1, &statement, nullptr) == SQLITE_OK)
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

	bool Store::updateCredential(const Credential& credential)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		bool res{ false };
		if (m_database)
		{
			sqlite3_stmt* statement;
			if (sqlite3_prepare_v2(m_database, "UPDATE credentials SET name = ?, uri = ?, username = ?, password = ? where id = ?", -1, &statement, nullptr) == SQLITE_OK)
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

	bool Store::deleteCredential(int id)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		bool res{ false };
		if (m_database)
		{
			sqlite3_stmt* statement;
			if (sqlite3_prepare_v2(m_database, "DELETE FROM credentials WHERE id = ?", -1, &statement, nullptr) == SQLITE_OK)
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

	bool Store::destroy()
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		if (m_database)
		{
			sqlite3_close(m_database);
			m_database = nullptr;
		}
		return std::filesystem::exists(m_path) ? std::filesystem::remove(m_path) : true;
	}

	Store& Store::operator=(const Store& store)
	{
		if (this != &store)
		{
			std::lock_guard<std::mutex> lock{ m_mutex };
			std::lock_guard<std::mutex> lock2{ store.m_mutex };
			m_name = store.m_name;
			m_password = store.m_password;
			m_database = store.m_database;
			m_path = store.m_path;
			loadDatabase();
		}
		return *this;
	}

	Store& Store::operator=(Store&& store) noexcept
	{
		if (this != &store)
		{
			std::lock_guard<std::mutex> lock{ m_mutex };
			std::lock_guard<std::mutex> lock2{ store.m_mutex };
			std::swap(m_name, store.m_name);
			std::swap(m_password, store.m_password);
			std::swap(m_database, store.m_database);
			std::swap(m_path, store.m_path);
			loadDatabase();
		}
		return *this;
	}

	void Store::loadDatabase()
	{
		if (sqlite3_open_v2(m_path.string().c_str(), &m_database, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) == SQLITE_OK)
		{
			if (sqlite3_key(m_database, m_password.c_str(), static_cast<int>(m_password.size())) == SQLITE_OK)
			{
				if (sqlite3_exec(m_database, "SELECT count(*) FROM sqlite_master;", nullptr, nullptr, nullptr) == SQLITE_OK)
				{
					if (sqlite3_exec(m_database, "CREATE TABLE IF NOT EXISTS credentials (id TEXT PRIMARY KEY, name TEXT, uri TEXT, username TEXT, password TEXT)", nullptr, nullptr, nullptr) == SQLITE_OK)
					{
						return;
					}
					std::cout << "Cannot create table" << std::endl;
				}
				std::cout << "Invalid key" << std::endl;
			}
			std::cout << "Cannot key" << std::endl;
			sqlite3_close(m_database);
		}
		std::cout << "Cannot open" << std::endl;
		m_database = nullptr;
	}

	bool Store::exists(const std::string& name)
	{
		return std::filesystem::exists(getPathFromName(name));
	}

	bool Store::destroy(const std::string& name)
	{
		std::filesystem::path path{ getPathFromName(name) };
		if (std::filesystem::exists(path))
		{
			return std::filesystem::remove(path);
		}
		return false;
	}
}