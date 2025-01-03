#include <gtest/gtest.h>
#include <memory>
#include "system/environment.h"
#include "system/process.h"

using namespace Nickvision::System;

class ProcessTest : public ::testing::Test
{
public:
    static std::unique_ptr<Process> m_proc;
};

std::unique_ptr<Process> ProcessTest::m_proc{ nullptr };

TEST_F(ProcessTest, Create)
{
#ifdef _WIN32
    ASSERT_NO_THROW(m_proc = std::make_unique<Process>(Environment::findDependency("cmd.exe"), std::vector<std::string>{ "/c", "timeout", "60" }));
#else
    ASSERT_NO_THROW(m_proc = std::make_unique<Process>(Environment::findDependency("sleep"), std::vector<std::string>{ "60" }));
#endif
    ASSERT_FALSE(m_proc->isRunning());
    ASSERT_FALSE(m_proc->hasCompleted());
}

TEST_F(ProcessTest, Start)
{
    ASSERT_TRUE(m_proc->start());
    ASSERT_TRUE(m_proc->isRunning());
    ASSERT_FALSE(m_proc->hasCompleted());
}

TEST_F(ProcessTest, Kill)
{
    ASSERT_TRUE(m_proc->kill());
    ASSERT_EQ(m_proc->waitForExit(), -1);
    ASSERT_FALSE(m_proc->isRunning());
    ASSERT_TRUE(m_proc->hasCompleted());
    ASSERT_EQ(m_proc->getExitCode(), -1);
}

TEST_F(ProcessTest, Destroy)
{
    ASSERT_NO_THROW(m_proc.reset());
}