#ifndef __TIMEQUEUE__H
#define __TIMEQUEUE__H

#include <set>
#include <vector>
#include <stdint.h>

#include "Timestamp.h"
#include "Callback.h"
#include "Channel.h"

namespace network
{
	class EventLoop;
	class Timer;
	class TimerId;
	

	class TimerQueue
	{
	public:
		TimerQueue(EventLoop* loop);
		~TimerQueue();	

		TimerId AddTimer(const TimerCallback& callback, Timestamp when, int64_t interval, int64_t repeat_count);

		// TimerId AddTimer(TimerCallback callback, Timestamp when, int64_t interval, int64_t repeat_count);

		void RemoveTimer(TimerId timer_id);
		void Cancel(TimerId timer_id, bool off);
		void DoTimer();
	private:
		TimerQueue(const TimerQueue&) = delete;
		TimerQueue& operator =(const TimerQueue&) = delete;

		void AddTimerInLoop(Timer* timer);
		void RemoveTimerInLoop(TimerId timer_id);
		void CancelTimerInLoop(TimerId timer_id, bool off);

		void Insert(Timer* timer);
	
	private:
		typedef std::pair<Timestamp, Timer*> 		Entry;
		typedef std::set<Entry>						TimerList;
		typedef std::pair<Timer*, int64_t>			ActiveTimer;
		typedef std::set<ActiveTimer>				ActiveTimerSet;	
		
		EventLoop* 	m_loop;
		TimerList	m_timer_list;
				
	};


}

#endif
