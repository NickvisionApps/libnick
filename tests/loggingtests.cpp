#include <gtest/gtest.h>
#include <fstream>
#include "logging/logger.h"

using namespace Nickvision::Logging;

class LoggingTest : public testing::Test
{
public:
    static std::unique_ptr<Logger> m_logger;

    static void SetUpTestSuite()
    {
        m_logger = std::make_unique<Logger>("log.log");
    }

    static void TearDownTestSuite()
    {
        m_logger.reset();
        std::filesystem::remove("log.log");
    }
};

std::unique_ptr<Logger> LoggingTest::m_logger = nullptr;

TEST_F(LoggingTest, Debug)
{
    ASSERT_NO_THROW(m_logger->log(LogLevel::Debug, "a"));
}

TEST_F(LoggingTest, Info)
{
    ASSERT_NO_THROW(m_logger->log(LogLevel::Info, "b"));
}

TEST_F(LoggingTest, Warning)
{
    ASSERT_NO_THROW(m_logger->log(LogLevel::Warning, "c"));
}

TEST_F(LoggingTest, Error)
{
    ASSERT_NO_THROW(m_logger->log(LogLevel::Error, "d"));
}

TEST_F(LoggingTest, Critical)
{
    ASSERT_NO_THROW(m_logger->log(LogLevel::Critical, "e"));
}

TEST_F(LoggingTest, EnsureMessages)
{
    std::ifstream in{ "log.log" };
    ASSERT_TRUE(in.is_open());
    std::string log((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    ASSERT_TRUE(log.find("[DEBUG]") != std::string::npos);
    ASSERT_TRUE(log.find("[INFO]") != std::string::npos);
    ASSERT_TRUE(log.find("[WARNING]") != std::string::npos);
    ASSERT_TRUE(log.find("[ERROR]") != std::string::npos);
    ASSERT_TRUE(log.find("[CRITICAL]") != std::string::npos);
}
