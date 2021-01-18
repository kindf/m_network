#include "TimerQueue.h"
#include "EventLoop.h"
#include "TimerId.h"
#include "Timer.h"

using namespace network;

namespace network
{
	TimerQueue::TimerQueue(EventLoop* loop):m_loop(loop),m_timer_list()
	{
	}


	TimerQueue::~TimerQueue()
	{
		for(TimerList::iterator it = m_timer_list.begin(); it != m_timer_list.end(); ++it)
		{
			delete it->second;
		}
	}


	TimerId TimerQueue::AddTimer(const TimerCallback& callback, Timestamp when, int64_t interval, int64_t repeat_count)
	{
		Timer* timer = new Timer(callback, when, interval, repeat_count);
		m_loop->RunInLoop(std::bind(&TimerQueue::AddTimerInLoop, this, timer));
		return TimerId(timer, timer->Sequence());
	}

	// TimerId TimerQueue::AddTimer(TimerCallback callback, Timestamp when, int64_t interval, int64_t repeat_count)
	// {
	// 	Timer* timer = new Timer(callback, when, interval, repeat_count);
	// 	m_loop->RunInLoop(std::bind(&TimerQueue::AddTimerInLoop, this, timer));
	// 	return TimerId(timer, timer->Sequence());
	// }

	void TimerQueue::RemoveTimer(TimerId timer_id)
	{
		m_loop->RunInLoop(std::bind(&TimerQueue::RemoveTimerInLoop, this, timer_id));
	}

	void TimerQueue::Cancel(TimerId timer_id, bool off)
	{
		m_loop->RunInLoop(std::bind(&TimerQueue::CancelTimerInLoop, this, timer_id, off));
	}

	void TimerQueue::DoTimer()
	{
		m_loop->AssertInLoopThread();
		Timestamp now(Timestamp::now());

		for(auto it = m_timer_list.begin(); it != m_timer_list.end();)
		{
			if(it->second->Expiration() <= now)
			{
				it->second->run();
				if(it->second->GetRepeatCount() == 0)
				{
					it = m_timer_list.erase(it);
				}
				else
				{
					++it;
				}
			}
			else
			{
				break;
			}
		}
	}

	void TimerQueue::AddTimerInLoop(Timer* timer)
	{
		m_loop->AssertInLoopThread();
		Insert(timer);
	}

	void TimerQueue::RemoveTimerInLoop(TimerId timer_id)
	{
		m_loop->AssertInLoopThread();
		
		Timer* timer = timer_id.m_timer;
		for(auto it = m_timer_list.begin(); it != m_timer_list.end(); ++it)
		{
			if(it->second == timer)
			{
				m_timer_list.erase(it);	
				break;
			}
		}
	}

	void TimerQueue::CancelTimerInLoop(TimerId timer_id, bool off)
	{
		m_loop->AssertInLoopThread();
		Timer* timer = timer_id.m_timer;
		for(auto it = m_timer_list.begin(); it != m_timer_list.end(); ++it)
		{
			if(timer == it->second)
			{
				it->second->Cancel(off);
				break;
			}
		}	
	}

	void TimerQueue::Insert(Timer* timer)
	{
		m_loop->AssertInLoopThread();
		Timestamp when = timer->Expiration();
		m_timer_list.insert(Entry(when, timer));
	}

}