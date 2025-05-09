#include <gtest/gtest.h>
#include "localization/gettext.h"

#define DOMAIN_NAME "libnick_test"

using namespace Nickvision::Localization;

TEST(LocalizationTests, Init)
{
    ASSERT_TRUE(Gettext::init(DOMAIN_NAME));
}

TEST(LocalizationTests, Domain)
{
    ASSERT_EQ(Gettext::getDomainName(), DOMAIN_NAME);
}

TEST(LocalizationTests, dgettext)
{
    ASSERT_EQ(std::string(_("Hello")), std::string("Hello"));
}

TEST(LocalizationTests, ngettext)
{
    ASSERT_EQ(std::string(_n("Hello", "Hellos", 2)), std::string("Hellos"));
}

TEST(LocalizationTests, fgettext)
{
    int count{ 1 };
    ASSERT_EQ(std::string(_f("Hello {} {}", "World", count)), std::string("Hello World 1"));
}

TEST(LocalizationTests, fngettext)
{
    int count{ 2 };
    ASSERT_EQ(std::string(_fn("Hello {}", "Hellos {}", count, "World")), std::string("Hellos World"));
}

TEST(LocalizationTests, pgettext)
{
    ASSERT_EQ(std::string(_p("Test", "Hello")), std::string("Hello"));
}

TEST(LocalizationTests, pngettext)
{
    ASSERT_EQ(std::string(_pn("Test", "Hello", "Hellos", 2)), std::string("Hellos"));
}
