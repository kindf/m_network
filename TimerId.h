#ifndef __TIMERID__H
#define __TIMERID__H

#include <stdint.h>
#include "Timer.h"

namespace network
{

	class Timer;

	class TimerId
	{
	public:
		TimerId():m_timer(NULL), m_sequence(0){}
		TimerId(Timer* timer, int64_t seq)
			:m_timer(timer), m_sequence(seq){}

		Timer* GetTimer()
		{
			return m_timer;
		}

		friend class TimerQueue;
	private:
		int64_t 	m_sequence;
		Timer* 		m_timer;	
	}	

}

#endif
