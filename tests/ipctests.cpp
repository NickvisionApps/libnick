#include <gtest/gtest.h>
#include "app/interprocesscommunicator.h"
#include "system/environment.h"

using namespace Nickvision::App;
using namespace Nickvision::Events;
using namespace Nickvision::System;

class IPCTest : public testing::Test
{
public:
    static std::unique_ptr<InterProcessCommunicator> m_ipc;
    static bool m_received;

    static void SetUpTestSuite()
    {
        m_ipc = std::make_unique<InterProcessCommunicator>("org.nickvision.aura.test");
        m_handlerId = m_ipc->commandReceived() += [](const ParamEventArgs<std::vector<std::string>>& e) 
        { 
            m_received = (e.getParam()[0] == "test1" && e.getParam()[1] == "test2" ? true : false);
        };
    }

    static void TearDownTestSuite()
    {
        m_ipc->commandReceived() -= m_handlerId;
        m_ipc.reset();
    }

private:
    static HandlerId m_handlerId;
};

std::unique_ptr<InterProcessCommunicator> IPCTest::m_ipc = nullptr;
bool IPCTest::m_received = false;
HandlerId IPCTest::m_handlerId = {};

TEST_F(IPCTest, EnsureServerStart)
{
    ASSERT_TRUE(m_ipc->isServer());
}

TEST_F(IPCTest, ClientSend)
{
    InterProcessCommunicator client{ "org.nickvision.aura.test" };
    ASSERT_TRUE(client.isClient());
    ASSERT_TRUE(client.communicate({ "test1", "test2" }));
}

TEST_F(IPCTest, EnsureServerReceived)
{
    while(!m_received)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
