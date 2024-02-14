# Nickvision::Database

This module provides object-oriented wrappers for working with sqlite3/sqlcipher.

## Table of Contents
- [SqlContext](#sqlcontext)
- [SqlDatabase](#sqldatabase)
- [SqlStatement](#sqlstatement)
- [SqlValue](#sqlvalue)

## SqlContext
Description: A sqlite function context.

Interface: [sqlcontent.h](/include/database/sqlcontent.h)

Path: `Nickvision::Database::SqlContext`

### Member Variables
- ```
  void* userData: get
  ```
    - The user data associated with the context.
- ```
  std::vector<SqlValue> args: get
  ```
    - The list of SqlValue arguments passed to the function.

### Methods
- ```cpp
  SqlContext(sqlite3_context* ctx, int argc, sqlite3_value** argv)
  ```
    - Constructs a SqlContext.
    - Accepts: The sqlite3 context pointer, ctx, the number of received arguments, argc, and the pointer to the list of sqlite3 values, argv.
- ```cpp
  void result()
  ```
    - Note: Returns a NULL value from the sql function.
- ```cpp
  void result(int value)
  ```
    - Accepts: An int value to return from the sql function, value.
- ```cpp
  void result(sqlite3_int64 value)
  ```
    - Accepts: An int64 value to return from the sql function, value.
- ```cpp
  void result(double value)
  ```
    - Accepts: A double value to return from the sql function, value.
- ```cpp
  void result(bool value)
  ```
    - Accepts: A bool value to return from the sql function, value.
- ```cpp
  void result(const std::string& value)
  ```
    - Accepts: A string value to return from the sql function, value.
- ```cpp
  void result(void* value, int n)
  ```
    - Accepts: A blob value to return from the sql function, value, and the size of said blob value, n.
- ```cpp
  void error(const std::string& err)
  ```
    - Accepts: An error message to return from the sql function, err.
- ```cpp
  void error(int err)
  ```
    - Accepts: An error cpode to return from the sql function, err.

## SqlDatabase
Description: A sqlite (sqlcipher) database.

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
  sqlite3* c_obj()
  ```
    - Returns: The underlying sqlite3 object pointer for the database.
    - Note: Using this method is strongly discouraged, as you can break the state of this object.
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
  void registerFunction(const std::string& name, const std::function<void(const SqlContext&)>& func, int expectedArgs = -1)
  ```
    - Accepts: The name of the sql function, name, the actual sql function, func, and the number of arguments the sql function expects to receive, expectedArgs (specify -1 for unlimited number of args).
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
Description: A sqlite statement.

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
    - Returns: The int value from the column.
- ```cpp
  sqlite3_int64 getColumnInt64(int index)
  ```
    - Accepts: The index of the column to get an int64 value from, index.
    - Returns: The int64 value from the column.
- ```cpp
  double getColumnDouble(int index)
  ```
    - Accepts: The index of the column to get a double value from, index.
    - Returns: The double value from the column.
- ```cpp
  bool getColumnBool(int index)
  ```
    - Accepts: The index of the column to get a bool value from, index.
    - Returns: The bool value from the column.
- ```cpp
  std::string getColumnString(int index)
  ```
    - Accepts: The index of the column to get a string value from, index.
    - Returns: The string value from the column.
- ```cpp
  std::pair<const void*, size_t> getColumnBlob(int index)
  ```
    - Accepts: The index of the column to get a blob value from, index.
    - Returns: A pair of the blob value from the column and the blob's size.
- ```cpp
  operator bool() const;
  ```
    - Returns: True if the object is valid.
    - Returns: False if the object is not valid.

## SqlValue
Description: A sqlite value.

Interface: [sqlvalue.h](/include/database/sqlvalue.h)

Path: `Nickvision::Database::SqlValue`

### Methods
- ```cpp
  SqlValue(sqlite3_value* value)
  ```
    - Constructs a SqlValue.
    - Accepts: The sqlite3 value pointer, value.
- ```cpp
  int getInt()
  ```
    - Returns: The int from the value.
- ```cpp
  sqlite3_int64 getInt64()
  ```
    - Returns: The int64 from the value.
- ```cpp
  double getDouble()
  ```
    - Returns: The double from the value.
- ```cpp
  bool getBool()
  ```
    - Returns: The bool from the value.
- ```cpp
  std::string getString()
  ```
    - Returns: The string from the value.
- ```cpp
  std::pair<const void*, size_t> getBlob()
  ```
    - Returns: A pair of the blob from the value and the blob's size.