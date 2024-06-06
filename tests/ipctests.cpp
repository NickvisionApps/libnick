#include <gtest/gtest.h>
#include <mutex>
#include "app/aura.h"
#include "app/interprocesscommunicator.h"
#include "system/environment.h"

using namespace Nickvision::App;
using namespace Nickvision::Events;
using namespace Nickvision::System;

static std::vector<std::string> args{ "test1", "test2" };

class IPCTest : public testing::Test
{
public:
    static void SetUpTestSuite()
    {
        Aura::getActive().init("org.nickvision.aura.test", "Nickvision Aura Tests", "Aura Tests");
        Aura::getActive().getIPC().commandReceived() += onCommandReceived;
    }

    static int getReceived()
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_received;
    }

private:
    static std::mutex m_mutex;
    static int m_received;

    static void onCommandReceived(const ParamEventArgs<std::vector<std::string>>& e)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        m_received += (e.getParam()[0] == args[0] && e.getParam()[1] == args[1] ? 1 : 0);
    }
};

std::mutex IPCTest::m_mutex = {};
int IPCTest::m_received = 0;

TEST_F(IPCTest, CheckServerStatus)
{
    ASSERT_TRUE(Aura::getActive().getIPC().isServer());
}

TEST_F(IPCTest, Client1Send)
{
    if (!Environment::getVariable("GITHUB_ACTIONS").empty())
    {
        ASSERT_TRUE(true);
        return;
    }
    InterProcessCommunicator client{ Aura::getActive().getAppInfo().getId() };
    ASSERT_TRUE(client.isClient());
    ASSERT_TRUE(client.communicate(args));
}

TEST_F(IPCTest, CheckServerReceived)
{
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ASSERT_TRUE(getReceived() > 0 || !Environment::getVariable("GITHUB_ACTIONS").empty());
}