#include <gtest/gtest.h>
#include <memory>
#include "database/sqldatabase.h"

using namespace Nickvision::Database;

class DatabaseTest : public testing::Test
{
public:
    static std::filesystem::path m_encryptedPath;
    static std::string m_encryptedPassword;

    static void SetUpTestSuite()
    {

    }

    static void TearDownTestSuite()
    {
        std::filesystem::remove(m_encryptedPath);
    }
};

std::filesystem::path DatabaseTest::m_encryptedPath = "enc.sqlite3";
std::string DatabaseTest::m_encryptedPassword = "abc3845@#$";

TEST_F(DatabaseTest, EncryptDatabase)
{
    std::unique_ptr<SqlDatabase> encrypted{ std::make_unique<SqlDatabase>(m_encryptedPath, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE) };
    ASSERT_FALSE(encrypted->isEncrypted());
    ASSERT_TRUE(encrypted->unlock(""));
    ASSERT_NO_THROW(encrypted->changePassword(m_encryptedPassword));
    encrypted.reset();
    ASSERT_NO_THROW(encrypted = std::make_unique<SqlDatabase>(m_encryptedPath, SQLITE_OPEN_READWRITE));
    ASSERT_TRUE(encrypted->isEncrypted());
    ASSERT_FALSE(encrypted->unlock(""));
    ASSERT_TRUE(encrypted->unlock(m_encryptedPassword));
}

TEST_F(DatabaseTest, DecryptDatabase)
{
    std::unique_ptr<SqlDatabase> encrypted{ std::make_unique<SqlDatabase>(m_encryptedPath, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE) };
    ASSERT_TRUE(encrypted->isEncrypted());
    ASSERT_TRUE(encrypted->unlock(m_encryptedPassword));
    ASSERT_NO_THROW(encrypted->changePassword(""));
    encrypted.reset();
    ASSERT_NO_THROW(encrypted = std::make_unique<SqlDatabase>(m_encryptedPath, SQLITE_OPEN_READWRITE));
    ASSERT_FALSE(encrypted->isEncrypted());
    ASSERT_TRUE(encrypted->unlock(""));
}