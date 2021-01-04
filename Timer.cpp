#include "Timer.h"

using namespace network;

namespace network
{
	Timer::Timer(const TimerCallback& callback, Timestamp when, int64_t interval, int64_t repeat_count)
	: m_callback(callback)
	, m_expiration(when)
	, m_interval(interval)
	, m_repeat_count(repeat_count)
	, m_canceled(false)
	{
		
	}

	void Timer::run()
	{
		if(m_canceled) return;
		
		m_callback();
		
		if(m_repeat_count != -1)
		{
			m_repeat_count--;
			if(m_repeat_count == 0)
			{
				return;
			}	
		}
		m_expiration += m_interval;
	}
}