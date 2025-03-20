#include <gtest/gtest.h>
#include "events/event.h"
#include "events/parameventargs.h"

using namespace Nickvision::Events;

TEST(EventTests, Event1)
{
    int count{ 0 };
    Event<ParamEventArgs<bool>> e;
    e += [&count](const ParamEventArgs<bool>& e)
    {
        if (*e)
        {
            count++;
        }
    };
    e += [&count](const ParamEventArgs<bool>& e)
    {
        if (!*e)
        {
            count--;
        }
    };
    e(true);
    e(false);
    e.invoke(true);
    ASSERT_EQ(e.count(), 2);
    ASSERT_EQ(count, 1);
}
