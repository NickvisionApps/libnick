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
    ASSERT_NO_THROW(m_proc = std::make_unique<Process>(Environment::findDependency("cmd.exe"), std::vector<std::string>{ "/c", "ping -n 60 127.0.0.1" }));
#else
    ASSERT_NO_THROW(m_proc = std::make_unique<Process>(Environment::findDependency("sleep"), std::vector<std::string>{ "60" }));
#endif
    ASSERT_EQ(m_proc->getState(), ProcessState::Created);
}

TEST_F(ProcessTest, Start)
{
    ASSERT_TRUE(m_proc->start());
    ASSERT_EQ(m_proc->getState(), ProcessState::Running);
}

#ifndef __APPLE__
TEST_F(ProcessTest, RAM)
{
    ASSERT_TRUE(m_proc->getRAMUsage() > 0);
}

TEST_F(ProcessTest, CPU)
{
    ASSERT_NO_THROW(m_proc->getCPUUsage());
}
#endif

TEST_F(ProcessTest, Pause)
{
    ASSERT_TRUE(m_proc->pause());
    ASSERT_EQ(m_proc->getState(), ProcessState::Paused);
    std::this_thread::sleep_for(std::chrono::seconds(8));
}

TEST_F(ProcessTest, Resume)
{
    ASSERT_TRUE(m_proc->resume());
    ASSERT_EQ(m_proc->getState(), ProcessState::Running);
}

TEST_F(ProcessTest, Kill)
{
    ASSERT_TRUE(m_proc->kill());
    ASSERT_EQ(m_proc->waitForExit(), -1);
    ASSERT_EQ(m_proc->getState(), ProcessState::Completed);
    ASSERT_EQ(m_proc->getExitCode(), -1);
}

TEST_F(ProcessTest, Destroy)
{
    ASSERT_NO_THROW(m_proc.reset());
}

TEST_F(ProcessTest, Send)
{
#ifdef _WIN32
    Process p{ Environment::findDependency("cmd.exe") };
#else
    Process p{ Environment::findDependency("sh") };
#endif
    ASSERT_TRUE(p.start());
    ASSERT_TRUE(p.sendCommand("echo Hello"));
    ASSERT_TRUE(p.sendCommand("exit"));
    p.waitForExit();
#ifdef _WIN32
    ASSERT_EQ(p.getOutput(), "Hello\r\n");
#else
    ASSERT_EQ(p.getOutput(), "Hello\n");
#endif
}
