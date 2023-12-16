#include <gtest/gtest.h>
#include <fstream>
#include "filesystem/filesystemwatcher.h"

using namespace Nickvision::Aura::Filesystem;

class FileWatcherTest : public testing::Test
{
public:
	static std::unique_ptr<FileSystemWatcher> m_watcher;
	static int m_modifications;

	static void SetUpTestSuite()
	{
		m_watcher = std::make_unique<FileSystemWatcher>(std::filesystem::current_path());
		m_watcher->changed() += [&](const FileSystemChangedEventArgs& e)
		{
			m_modifications++;
		};
	}
};

std::unique_ptr<FileSystemWatcher> FileWatcherTest::m_watcher = nullptr;
int FileWatcherTest::m_modifications = 0;

static std::filesystem::path a{ std::filesystem::current_path() / "a.txt" };
static std::filesystem::path b{ std::filesystem::current_path() / "b.txt" };
static std::filesystem::path c{ std::filesystem::current_path() / "c.txt" };

TEST_F(FileWatcherTest, AddFileA)
{	
	std::ofstream out{ a };
	out << "a";
	ASSERT_NO_THROW(out.close());
}

TEST_F(FileWatcherTest, AddFileB)
{
	std::ofstream out{ b };
	out << "b";
	ASSERT_NO_THROW(out.close());
}

TEST_F(FileWatcherTest, AddFileC)
{
	std::ofstream out{ c };
	out << "c";
	ASSERT_NO_THROW(out.close());
}

TEST_F(FileWatcherTest, Cleanup)
{
	ASSERT_TRUE(std::filesystem::remove(a));
	ASSERT_TRUE(std::filesystem::remove(b));
	ASSERT_TRUE(std::filesystem::remove(c));
}

TEST_F(FileWatcherTest, CheckResults)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	ASSERT_TRUE(m_modifications, 3);
}