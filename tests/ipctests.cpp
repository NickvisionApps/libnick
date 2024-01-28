#include <gtest/gtest.h>
#include <mutex>
#include "aura/aura.h"
#include "aura/interprocesscommunicator.h"

using namespace Nickvision::Aura;
using namespace Nickvision::Events;

static std::vector<std::string> args{ "test1", "test2" };

class IPCTest : public testing::Test
{
public:
	static std::unique_ptr<InterProcessCommunicator> m_server;

	static void SetUpTestSuite()
	{
		Aura::getActive().init("org.nickvision.aura.test", "Nickvision Aura Tests", "Aura Tests");
		m_server = std::make_unique<InterProcessCommunicator>();
		m_server->commandReceived() += onCommandReceived;
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
std::unique_ptr<InterProcessCommunicator> IPCTest::m_server = nullptr;

TEST_F(IPCTest, CheckServerStatus)
{
	ASSERT_TRUE(m_server->isServer());
}

TEST_F(IPCTest, Client1Send)
{
	if (Aura::getActive().getEnvVar("GITHUB_ACTIONS") == "true")
	{
		ASSERT_TRUE(true);
		return;
	}
	InterProcessCommunicator client;
	ASSERT_TRUE(client.isClient());
	ASSERT_TRUE(client.communicate(args));
}

TEST_F(IPCTest, CheckServerReceived)
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	ASSERT_TRUE(getReceived() > 0 || Aura::getActive().getEnvVar("GITHUB_ACTIONS") == "true");
}

TEST_F(IPCTest, Cleanup)
{
	ASSERT_NO_THROW(m_server.reset());
}