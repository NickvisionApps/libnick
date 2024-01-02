#include <gtest/gtest.h>
#include "events/event.h"

using namespace Nickvision::Aura::Events;

class StateChangedEventArgs : EventArgs
{
public:
	StateChangedEventArgs(bool state)
		: m_state{ state }
	{

	}

	bool getState() const
	{
		return m_state;
	}

private:
	bool m_state;
};

TEST(EventTests, Event1)
{
	int count{ 0 };
	Event<StateChangedEventArgs> e;
	e += [&count](const StateChangedEventArgs& e)
	{
		if (e.getState())
		{
			count++;
		}
	};
	e += [&count](const StateChangedEventArgs& e)
	{
		if (!e.getState())
		{
			count--;
		}
	};
	e(true);
	e(false);
	e.invoke(true);
	ASSERT_EQ(count, 1);
}