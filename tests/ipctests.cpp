#include <gtest/gtest.h>
#include "ipcserver.h"

using namespace Nickvision::Aura;
using namespace Nickvision::Aura::Events;

class IPCServerTest : public testing::Test
{
public:
	static std::unique_ptr<IPCServer> m_server;
	static bool m_received;
	static std::vector<std::string> m_args;

	static void SetUpTestSuite()
	{
		m_server = std::make_unique<IPCServer>();
		m_server->commandReceived() += onCommandReceived;
	}

private:
	static void onCommandReceived(const ParamEventArgs<std::vector<std::string>>& e)
	{
		m_received = e.getParam()[0] == m_args[0] && e.getParam()[1] == m_args[1];
	}
};

std::unique_ptr<IPCServer> IPCServerTest::m_server = nullptr;
bool IPCServerTest::m_received = false;
std::vector<std::string> IPCServerTest::m_args = { "test1", "test2" };

TEST_F(IPCServerTest, StartServer)
{
	ASSERT_FALSE(m_server->communicate(m_args));
}

TEST_F(IPCServerTest, SendArgs)
{
	ASSERT_TRUE(m_server->communicate(m_args));
	std::this_thread::sleep_for(std::chrono::seconds(1));
	ASSERT_TRUE(m_received);
}

TEST_F(IPCServerTest, Cleanup)
{
	ASSERT_NO_THROW(m_server.reset());
}