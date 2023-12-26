#include <gtest/gtest.h>
#include "interprocesscommunicator.h"
#include <mutex>

using namespace Nickvision::Aura;
using namespace Nickvision::Aura::Events;

static std::vector<std::string> args{ "test1", "test2" };

class IPCTest : public testing::Test
{
public:
	static std::mutex m_mutex;
	static std::unique_ptr<InterProcessCommunicator> m_server;
	static int m_received;

	static int getReceived()
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		return m_received;
	}

	static void onCommandReceived(const ParamEventArgs<std::vector<std::string>>& e)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		m_received += (e.getParam()[0] == args[0] && e.getParam()[1] == args[1] ? 1 : 0);
	}
};

std::mutex IPCTest::m_mutex = {};
std::unique_ptr<InterProcessCommunicator> IPCTest::m_server = nullptr;
int IPCTest::m_received = 0;

TEST_F(IPCTest, CheckServerStart)
{
	ASSERT_NO_THROW(m_server = std::make_unique<InterProcessCommunicator>());
	m_server->commandReceived() += onCommandReceived;
	ASSERT_TRUE(m_server->isServer());
}

TEST_F(IPCTest, Client1Send)
{
	InterProcessCommunicator client;
	ASSERT_TRUE(client.isClient());
	ASSERT_TRUE(client.communicate(args));
}

TEST_F(IPCTest, CheckServerReceived)
{
	ASSERT_TRUE(getReceived() > 0);
}

TEST_F(IPCTest, Cleanup)
{
	ASSERT_NO_THROW(m_server.reset());
}