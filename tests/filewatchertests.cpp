#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include "filesystem/filesystemwatcher.h"

using namespace Nickvision::Aura::Filesystem;

class FileWatcherTest : public testing::Test
{
public:
	static std::unique_ptr<FileSystemWatcher> m_watcher;
	static int m_modifications;

	static void SetUpTestSuite()
	{
		m_watcher = std::make_unique<FileSystemWatcher>(std::filesystem::current_path(), false);
		m_watcher->addExtensionFilter(".txt");
		m_watcher->changed() += onChanged;
	}

private:
	static void onChanged(const FileSystemChangedEventArgs& e)
	{
		m_modifications++;
	}
};

std::unique_ptr<FileSystemWatcher> FileWatcherTest::m_watcher = nullptr;
int FileWatcherTest::m_modifications = 0;

static std::filesystem::path a{ "a.txt" };
static std::filesystem::path b{ "b.md" };
static std::filesystem::path c{ "c.txt" };

TEST_F(FileWatcherTest, AddFileA)
{
	std::ofstream out{ a };
	ASSERT_NO_THROW(out.close());
}

TEST_F(FileWatcherTest, AddFileB)
{
	std::ofstream out{ b };
	ASSERT_NO_THROW(out.close());
}

TEST_F(FileWatcherTest, AddFileC)
{
	std::ofstream out{ c };
	ASSERT_NO_THROW(out.close());
}

TEST_F(FileWatcherTest, CheckResults)
{
	ASSERT_TRUE(m_modifications >= 1);
}

TEST_F(FileWatcherTest, Cleanup)
{
	ASSERT_NO_THROW(m_watcher.reset());
	ASSERT_TRUE(std::filesystem::remove(a));
	ASSERT_TRUE(std::filesystem::remove(b));
	ASSERT_TRUE(std::filesystem::remove(c));
}