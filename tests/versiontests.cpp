#include <gtest/gtest.h>
#include "version.h"

TEST(VersionTests, EmptyVersion) 
{
	Nickvision::Aura::Version v;
	EXPECT_EQ(v.empty(), true);
}