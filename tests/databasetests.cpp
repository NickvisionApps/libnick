#include <gtest/gtest.h>
#include <memory>
#include "database/sqldatabase.h"

using namespace Nickvision::Database;

TEST(DatabaseTests, EncryptedDatabase)
{
    std::filesystem::path path{ "enc.sqlite3" };
    std::string password{ "abc72356" };
    std::unique_ptr<SqlDatabase> encrypted{ std::make_unique<SqlDatabase>(path, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE) };
    ASSERT_FALSE(encrypted->isEncrypted());
    ASSERT_TRUE(encrypted->unlock(""));
    ASSERT_NO_THROW(encrypted->changePassword(password));
    encrypted.reset();
    ASSERT_NO_THROW(encrypted = std::make_unique<SqlDatabase>(path, SQLITE_OPEN_READWRITE));
    ASSERT_TRUE(encrypted->isEncrypted());
    ASSERT_FALSE(encrypted->unlock(""));
    ASSERT_TRUE(encrypted->unlock(password));
}