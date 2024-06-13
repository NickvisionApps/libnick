#include <gtest/gtest.h>
#include <mutex>
#include "app/aura.h"
#include "app/interprocesscommunicator.h"
#include "system/environment.h"

using namespace Nickvision::App;
using namespace Nickvision::Events;
using namespace Nickvision::System;

class IPCTest : public testing::Test
{
public:
    static std::unique_ptr<InterProcessCommunicator> m_ipc;

    static void SetUpTestSuite()
    {
        m_ipc = std::make_unique<InterProcessCommunicator>("org.nickvision.aura.test");
        m_handlerId = m_ipc->commandReceived() += [](const ParamEventArgs<std::vector<std::string>>& e) 
        { 
            std::lock_guard<std::mutex> lock{ m_mutex };
            m_received = (e.getParam()[0] == "test1" && e.getParam()[1] == "test2" ? true : false);
        };
    }

    static void TearDownTestSuite()
    {
        m_ipc->commandReceived() -= m_handlerId;
    }

    static bool getReceived()
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_received;
    }

private:
    static std::mutex m_mutex;
    static bool m_received;
    static HandlerId m_handlerId;
};

std::unique_ptr<InterProcessCommunicator> IPCTest::m_ipc = nullptr;
std::mutex IPCTest::m_mutex = {};
bool IPCTest::m_received = false;
HandlerId IPCTest::m_handlerId = 0;

TEST_F(IPCTest, CheckServerStatus)
{
    ASSERT_TRUE(m_ipc->isServer());
}

TEST_F(IPCTest, Client1Send)
{
    InterProcessCommunicator client{ "org.nickvision.aura.test" };
    ASSERT_TRUE(client.isClient());
    ASSERT_TRUE(client.communicate({ "test1", "test2" }));
}

TEST_F(IPCTest, CheckServerReceived)
{
    while(!getReceived())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    ASSERT_TRUE(getReceived());
}