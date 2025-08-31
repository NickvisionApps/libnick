#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "database/sqlitedatabase.h"

using namespace Nickvision::Database;

class Person
{
public:
    Person(const std::string& name, int age) noexcept
        : m_name{ name },
        m_age{ age }
    {

    }

    const std::string& getName() const noexcept
    {
        return m_name;
    }

    int getAge() const noexcept
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
    static std::filesystem::path m_path;
    static std::unique_ptr<SqliteDatabase> m_database;

    static void TearDownTestSuite()
    {
        m_database.reset();
        std::filesystem::remove(m_path);
    }

    static const Person& getPerson1()
    {
        static Person one{ "Bob Hope", 67 };
        return one;
    }

    static const Person& getPerson2()
    {
        static Person two{ "Chris Pratt", 32 };
        return two;
    }
};

std::filesystem::path DatabaseTest::m_path = "enc.sqlite3";
std::unique_ptr<SqliteDatabase> DatabaseTest::m_database = nullptr;

TEST_F(DatabaseTest, OpenDatabase)
{
    ASSERT_NO_THROW(m_database = std::make_unique<SqliteDatabase>(m_path));
    ASSERT_TRUE(m_database->isUnlocked());
    ASSERT_FALSE(m_database->isEncrypted());
}

TEST_F(DatabaseTest, AddTable)
{
    ASSERT_TRUE(m_database->execute("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT, age INTEGER)"));
}

TEST_F(DatabaseTest, EncryptDatabase)
{
    ASSERT_TRUE(m_database->setPassword("abc3845@#$"));
    ASSERT_TRUE(m_database->isEncrypted());
    ASSERT_TRUE(m_database->isUnlocked());
}

TEST_F(DatabaseTest, AddPerson1)
{
    SqliteStatement statement{ m_database->createStatement("INSERT INTO people (id, name, age) VALUES (?,?,?)") };
    statement.bind(1, 1);
    statement.bind(2, getPerson1().getName());
    statement.bind(3, getPerson1().getAge());
    ASSERT_EQ(statement.step(), SqliteStepResult::Done);
}

TEST_F(DatabaseTest, AddPerson2)
{
    SqliteStatement statement{ m_database->createStatement("INSERT INTO people (id, name, age) VALUES (?,?,?)") };
    statement.bind(1, 2);
    statement.bind(2, getPerson2().getName());
    statement.bind(3, getPerson2().getAge());
    ASSERT_EQ(statement.step(), SqliteStepResult::Done);
}

TEST_F(DatabaseTest, GetPerson1)
{
    SqliteStatement statement{ m_database->createStatement("SELECT * FROM people WHERE id = 1") };
    ASSERT_EQ(statement.step(), SqliteStepResult::Row);
    ASSERT_EQ(statement.getColumn<int>(0), 1);
    ASSERT_EQ(statement.getColumn<std::string>(1), getPerson1().getName());
    ASSERT_EQ(statement.getColumn<int>(2), getPerson1().getAge());
    ASSERT_EQ(statement.step(), SqliteStepResult::Done);
}

TEST_F(DatabaseTest, DecryptDatabase)
{
    ASSERT_TRUE(m_database->setPassword(""));
    ASSERT_FALSE(m_database->isEncrypted());
    ASSERT_TRUE(m_database->isUnlocked());
}

TEST_F(DatabaseTest, GetPerson2)
{
    SqliteStatement statement{ m_database->createStatement("SELECT * FROM people WHERE id = 2") };
    ASSERT_EQ(statement.step(), SqliteStepResult::Row);
    ASSERT_EQ(statement.getColumn<int>(0), 2);
    ASSERT_EQ(statement.getColumn<std::string>(1), getPerson2().getName());
    ASSERT_EQ(statement.getColumn<int>(2), getPerson2().getAge());
    ASSERT_EQ(statement.step(), SqliteStepResult::Done);
}