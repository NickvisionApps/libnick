#include <gtest/gtest.h>
#include "helpers/sizehelpers.h"
#include "system/hardwareinfo.h"

using namespace Nickvision::Helpers;
using namespace Nickvision::System;

TEST(HardwareTests, CPU)
{
    unsigned int proc{ HardwareInfo::getNumberOfProcessors() };
    ASSERT_TRUE(proc > 0);
}

TEST(HardwareTests, TotalRam)
{
    unsigned long long ram{ HardwareInfo::getTotalRamSize() };
    ASSERT_TRUE(ram > 0);
    ASSERT_EQ(SizeHelpers::bytesToMegabytes(ram), ram / 1024 / 1024);
    ASSERT_EQ(SizeHelpers::bytesToGigabytes(ram), ram / 1024 / 1024 / 1024);
}

TEST(HardwareTests, FreeRam)
{
    unsigned long long ram{ HardwareInfo::getFreeRamSize() };
    ASSERT_TRUE(ram > 0);
    ASSERT_EQ(SizeHelpers::bytesToMegabytes(ram), ram / 1024 / 1024);
    ASSERT_EQ(SizeHelpers::bytesToGigabytes(ram), ram / 1024 / 1024 / 1024);
}
