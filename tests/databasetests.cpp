#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "database/sqldatabase.h"

using namespace Nickvision::Database;

class Person
{
public:
    Person(const std::string& name, int age)
        : m_name{ name },
        m_age{ age }
    {

    }

    const std::string& getName() const
    {
        return m_name;
    }

    int getAge() const
    {
        return m_age;
    }

private:
    std::string m_name;
    int m_age;
};

class DatabaseTest : public testing::Test
{
public:
    static std::filesystem::path m_encryptedPath;
    static std::string m_encryptedPassword;
    static std::unique_ptr<SqlDatabase> m_encrypted;

    static void SetUpTestSuite()
    {
        m_encrypted = std::make_unique<SqlDatabase>(m_encryptedPath, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    }

    static void TearDownTestSuite()
    {
        m_encrypted.reset();
        std::filesystem::remove(m_encryptedPath);
    }

    static Person getPerson1()
    {
        return { "Bob Hope", 67 };
    }

    static Person getPerson2()
    {
        return { "Chris Pratt", 32 };
    }
};

std::filesystem::path DatabaseTest::m_encryptedPath = "enc.sqlite3";
std::string DatabaseTest::m_encryptedPassword = "abc3845@#$";
std::unique_ptr<SqlDatabase> DatabaseTest::m_encrypted = nullptr;

TEST_F(DatabaseTest, EncryptDatabase)
{
    ASSERT_FALSE(m_encrypted->isEncrypted());
    ASSERT_TRUE(m_encrypted->unlock(""));
    ASSERT_TRUE(m_encrypted->changePassword(m_encryptedPassword));
    ASSERT_TRUE(m_encrypted->isEncrypted());
}

TEST_F(DatabaseTest, AddTable)
{
    ASSERT_TRUE(m_encrypted->exec("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT, age INTEGER)"));
}

TEST_F(DatabaseTest, AddPerson1)
{
    Person p1{ getPerson1() };
    SqlStatement statement{ m_encrypted->createStatement("INSERT INTO people (id, name, age) VALUES (?,?,?)") };
    statement.bind(1, 1);
    statement.bind(2, p1.getName());
    statement.bind(3, p1.getAge());
    ASSERT_FALSE(statement.step());
}

TEST_F(DatabaseTest, AddPerson2)
{
    Person p2{ getPerson2() };
    SqlStatement statement{ m_encrypted->createStatement("INSERT INTO people (id, name, age) VALUES (?,?,?)") };
    statement.bind(1, 2);
    statement.bind(2, p2.getName());
    statement.bind(3, p2.getAge());
    ASSERT_FALSE(statement.step());
}

TEST_F(DatabaseTest, GetPerson1)
{
    Person p1{ getPerson1() };
    SqlStatement statement{ m_encrypted->createStatement("SELECT * FROM people WHERE id = 1") };
    ASSERT_TRUE(statement.step());
    ASSERT_EQ(statement.getColumnInt(0), 1);
    ASSERT_EQ(statement.getColumnString(1), p1.getName());
    ASSERT_EQ(statement.getColumnInt(2), p1.getAge());
    ASSERT_FALSE(statement.step());
}

TEST_F(DatabaseTest, GetPerson2)
{
    Person p2{ getPerson2() };
    SqlStatement statement{ m_encrypted->createStatement("SELECT * FROM people WHERE id = 2") };
    ASSERT_TRUE(statement.step());
    ASSERT_EQ(statement.getColumnInt(0), 2);
    ASSERT_EQ(statement.getColumnString(1), p2.getName());
    ASSERT_EQ(statement.getColumnInt(2), p2.getAge());
    ASSERT_FALSE(statement.step());
}

TEST_F(DatabaseTest, DecryptDatabase)
{
    ASSERT_TRUE(m_encrypted->changePassword(""));
    ASSERT_FALSE(m_encrypted->isEncrypted());
    SqlStatement statement{ m_encrypted->createStatement("SELECT COUNT(id) FROM people") };
    ASSERT_TRUE(statement.step());
    ASSERT_EQ(statement.getColumnInt(0), 2);
}