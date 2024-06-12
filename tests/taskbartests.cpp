#include <gtest/gtest.h>
#include "taskbar/taskbaritem.h"

using namespace Nickvision::Taskbar;

class TaskbarTest : public testing::Test 
{
public:
    static std::unique_ptr<TaskbarItem> m_taskbar;

    static void SetUpTestSuite()
    {
        m_taskbar = std::make_unique<TaskbarItem>();
    }
};

std::unique_ptr<TaskbarItem> TaskbarTest::m_taskbar = nullptr;

#ifdef _WIN32
TEST_F(TaskbarTest, ConnectWindows)
{
    HWND hwnd{ GetConsoleWindow() };
    if (hwnd)
    {
        ASSERT_TRUE(m_taskbar->connect(hwnd));
    }
}
#elif defined(__linux__)
TEST_F(TaskbarTest, ConnectLinux)
{
    ASSERT_TRUE(m_taskbar->connect("firefox.desktop"));
}
#endif

TEST_F(TaskbarTest, SetProgress)
{
    ASSERT_NO_THROW(m_taskbar->setProgress(0.35));
    ASSERT_TRUE(m_taskbar->getProgressState() == ProgressState::Normal);
}

TEST_F(TaskbarTest, SetProgressPaused)
{
    ASSERT_NO_THROW(m_taskbar->setProgressState(ProgressState::Paused));
}

TEST_F(TaskbarTest, SetUrgent)
{
    ASSERT_NO_THROW(m_taskbar->setUrgent(true));
}

TEST_F(TaskbarTest, SetCount)
{
    ASSERT_NO_THROW(m_taskbar->setCountVisible(true));
    ASSERT_NO_THROW(m_taskbar->setCount(5));
}

TEST_F(TaskbarTest, Cleanup)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ASSERT_NO_THROW(m_taskbar.reset());
}