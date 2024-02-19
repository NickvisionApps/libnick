#include <gtest/gtest.h>
#include <fstream>
#include <mutex>
#include "filesystem/filesystemwatcher.h"

using namespace Nickvision::Filesystem;

class FileWatcherTest : public testing::Test
{
public:
    static std::unique_ptr<FileSystemWatcher> m_watcher;

    static void SetUpTestSuite()
    {
        m_watcher = std::make_unique<FileSystemWatcher>(std::filesystem::current_path(), false);
        m_watcher->addExtensionFilter(".txt");
        m_watcher->changed() += onChanged;
    }

    static int getModifications()
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_modifications;
    }

private:
    static std::mutex m_mutex;
    static int m_modifications;

    static void onChanged(const FileSystemChangedEventArgs& e)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        m_modifications++;
    }
};

std::mutex FileWatcherTest::m_mutex = {};
int FileWatcherTest::m_modifications = 0;
std::unique_ptr<FileSystemWatcher> FileWatcherTest::m_watcher = nullptr;

static std::filesystem::path a{ "a.txt" };
static std::filesystem::path b{ "b.md" };
static std::filesystem::path c{ "c.txt" };

TEST_F(FileWatcherTest, AddFileA)
{
    std::ofstream out{ a };
    ASSERT_NO_THROW(out.close());
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

TEST_F(FileWatcherTest, AddFileB)
{
    std::ofstream out{ b };
    ASSERT_NO_THROW(out.close());
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

TEST_F(FileWatcherTest, AddFileC)
{
    std::ofstream out{ c };
    ASSERT_NO_THROW(out.close());
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

TEST_F(FileWatcherTest, CheckResults)
{
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ASSERT_TRUE(getModifications() > 0);
}

TEST_F(FileWatcherTest, Cleanup)
{
    ASSERT_NO_THROW(m_watcher.reset());
    ASSERT_TRUE(std::filesystem::remove(a));
    ASSERT_TRUE(std::filesystem::remove(b));
    ASSERT_TRUE(std::filesystem::remove(c));
}