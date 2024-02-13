# Nickvision::Database

This module provides object-oriented wrappers for working with sqlite3/sqlcipher.

## Table of Contents
- [SqlDatabase](#sqldatabase)
- [SqlStatement](#sqlstatement)

## SqlDatabase
Description: An sqlite (sqlcipher) database.

Interface: [sqldatabase.h](/include/database/sqldatabase.h)

Path: `Nickvision::Database::SqlDatabase`

### Member Variables
- ```
  std::filesystem::path Path: get
  ```
    - The path of the database file.
- ```
  bool IsEncrypted: get
  ```
    - Whether or not the database is encrypted.

### Methods
- ```cpp
  SqlDatabase(const std::filesystem::path& path, int flags)
  ```
    - Constructs a SqlDatabase
    - Accepts: The path to the database file, path and the flags to pass when opening the database, flags.
    - Throws: `std::runtime_error` if there is an error in executing the sql statement.
- ```cpp
  SqlDatabase(const SqlDatabase& database)
  ```
    - Copies a SqlDatabase.
- ```cpp
  SqlDatabase(SqlDatabase&& database)
  ```
    - Moves a SqlDatabase.
- ```cpp
  bool unlock(const std::string& password)
  ```
    - Accepts: The password of the database, password.
    - Returns: True if unlocked.
    - Returns: False if not unlocked.
    - Throws: `std::runtime_error` if there is an error in executing the sql statement.
    - Note: If the database is not encrypted and is newly created, this will encrypt and set its password.
- ```cpp
  bool changePassword(const std::string& password)
  ```
    - Accepts: The new password of the database, password.
    - Returns: True if password changed.
    - Returns: False if password not changed.
    - Note: Passing an empty string for the password will unencrypt the database.
    - Note: Changing the password on an encrypted database requires that the database be unlocked first, even if removing the password.
- ```cpp
  bool exec(const std::string& command)
  ```
    - Accepts: The sql command to execute in the database, command.
    - Returns: True if the command returned SQLITE_OK.
    - Returns: False if the common did not return SQLITE_OK.
- ```cpp
  SqlStatement createStatement(const std::string& command)
  ```
    - Accepts: The sql command to bind to the statement, command.
    - Returns: The new SqlStatement.
    - Throws: `std::runtime_error` if there is an error in executing the sql statement.
- ```cpp
  SqlDatabase& operator=(const SqlDatabase& database)
  ```
    - Copies a SqlDatabase.
- ```cpp
  SqlDatabase& operator=(SqlDatabase&& database)
  ```
    - Moves a SqlDatabase.
- ```cpp
  operator bool() const
  ```
    - Returns: True if the object is valid, i.e. isEncrypted ? isUnlocked : true.
    - Returns: False if the object is not valid.

## SqlStatement
Description: An sqlite (sqlcipher) statement.

Interface: [sqldstatement.h](/include/database/sqlstatement.h)

Path: `Nickvision::Database::SqlStatement`

### Methods
- ```cpp
  SqlStatement(const std::shared_ptr<sqlite3>& database, const std::string& command)
  ```
    - Constructs a SqlStatement
    - Accepts: A pointer to an sqlite3 database, database, and the sql command to bind to the statement, command.
    - Throws: `std::runtime_error` if there is an error in executing the sql statement.
- ```cpp
  void bind(int index, int value)
  ```
    - Accepts: The index of the parameter to bind to, index, and the int value to bind to the parameter, value.
    - Throws: `std::runtime_error` if there is an error in executing the sql statement.
- ```cpp
  void bind(int index, sqlite3_int64 value)
  ```
    - Accepts: The index of the parameter to bind to, index, and the int64 value to bind to the parameter, value.
    - Throws: `std::runtime_error` if there is an error in executing the sql statement.
- ```cpp
  void bind(int index, double value)
  ```
    - Accepts: The index of the parameter to bind to, index, and the double value to bind to the parameter, value.
    - Throws: `std::runtime_error` if there is an error in executing the sql statement.
- ```cpp
  void bind(int index, bool value)
  ```
    - Accepts: The index of the parameter to bind to, index, and the bool value to bind to the parameter, value.
    - Throws: `std::runtime_error` if there is an error in executing the sql statement.
- ```cpp
  void bind(int index, const std::string& value)
  ```
    - Accepts: The index of the parameter to bind to, index, and the string value to bind to the parameter, value.
    - Throws: `std::runtime_error` if there is an error in executing the sql statement.
- ```cpp
  void bind(int index, void* value, int n)
  ```
    - Accepts: The index of the parameter to bind to, index, the blob value to bind to the parameter, value, and the size of the blob value, n.
    - Throws: `std::runtime_error` if there is an error in executing the sql statement.
- ```cpp
  bool step()
  ```
    - Returns: True if stepping through the statement returns SQLITE_ROW.
    - Returns: False if stepping though the statement does not return SQLITE_ROW, i.e. SQLITE_DONE.
    - Throws: `std::runtime_error` if there is an error in executing the sql statement.
- ```cpp
  int getColumnInt(int index)
  ```
    - Accepts: The index of the column to get an int value from, index.
    - Return The int value from the column.
- ```cpp
  sqlite3_int64 getColumnInt64(int index)
  ```
    - Accepts: The index of the column to get an int64 value from, index.
    - Return The int64 value from the column.
- ```cpp
  double getColumnDouble(int index)
  ```
    - Accepts: The index of the column to get a double value from, index.
    - Return The double value from the column.
- ```cpp
  bool getColumnBool(int index)
  ```
    - Accepts: The index of the column to get a bool value from, index.
    - Return The bool value from the column.
- ```cpp
  std::string getColumnString(int index)
  ```
    - Accepts: The index of the column to get a string value from, index.
    - Return The string value from the column.
- ```cpp
  std::pair<const void*, int> getColumnBlob(int index)
  ```
    - Accepts: The index of the column to get a blob value from, index.
    - Returns: A pair of the blob value from the column and the blob's size.
- ```cpp
  operator bool() const;
  ```
    - Returns: True if the object is valid.
    - Returns: False if the object is not valid.