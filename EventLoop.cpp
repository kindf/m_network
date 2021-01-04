#include "EventLoop.h"
#include <sys/eventfd.h>
#include <cassert>

using namespace network;

namespace network
{
	EventLoop::EventLoop()
	:m_looping(false)
	,m_quit(false)
	,m_tid(std::this_thread::get_id())
	,m_cur_active_channel(NULL)
	,m_timer_queue(new TimerQueue(this))
	{
		m_epoller.reset(new Epoller(this));
		CreateWakeUpFd();
		m_wakeup_channel.reset(new Channel(this, m_wakeup_fd);
		m_wakeup_channel->SetReadCallBack(std::bind(&EventLoop::HandleRead, this);
		m_wakeup_channel->SetReadEvent();	
	}


	EventLoop::Loop()
	{
		assert(IsInLoopThread);
		while(!m_quit)
		{
			m_timer_queue->DoTimer();
			m_cur_active_channel.clear();
			m_epoller->Poll(1, m_channel_vec);
			for(const auto& it : m_channel_vec)
			{
				m_cur_active_channel = it;
				m_cur_active_channel->HandleEvent();
			}
			DoPendingFunc();
		}
	}

	void EventLoop::DoPendingFunc()
	{
		std::vector<Func> functors;
		m_calling_pending_func = true;
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			functors.swap(m_pending_func);
		}
		for(auto it = functors.begin(); it != functors.end(); it++)
		{
			(*it)();	
		}
		m_calling_pending_func = false;
	}

	void EventLoop::RunInLoop(Func& func)
	{
		if(IsInLoopThread())
		{
			func();
		}
		else
		{
			QueueInLoop(func);
		}
	}

	void EventLoop::AssertInLoopThread()
	{
		assert(IsInLoopThread());
	}

	void EventLoop::QueueInLoop(Func& func)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_peding_func.push_back(func);
		}

		if(!IsInLoopThread() || m_calling_pending_func)
		{
			WakeUp();
		}
	}

	void EventLoop::Quit()
	{
		m_quit = true;
		if(!IsInLoopThread())
		{
			WakeUp();
		}
	}

	TimerId EventLoop::RunAt(const Timestamp& when, const TimerCallback& cb)
	{
		return m_timer_queue->AddTimer(cb, when, 0, 1);	
	}

	TimerId EventLoop::RunAfter(int64_t delay, const TimerCallback& cb)
	{
		Timestamp time(AddTimer(Timestamp::now(), delay));
		return RunAt(tim, cb);
	}

	TimerId EventLoop::RunEvery(int64_t interval, const TimerCallback& cb)
	{
		Timestamp time(AddTimer(Timestamp::now(), interval);
		return m_timer_queue->AddTimer(cb, time, interval, -1);	
	}

	bool EventLoop::CreateWakeUpFd()
	{
		m_wakeup_fd = ::eventfd(0, EFD_CLOEXEC|EFD_NONBLOCK);
		assert(m_wakeup_fd >= 0);
		return true;
	}

	bool EventLoop::WakeUp()
	{
		uint64_t wdata = 0;
		int n = ::write(m_wakeup_fd, &wdata, sizeof(wdata));
		assert(n != sizeof(wdata);
		return true;
	}

	bool EventLoop::HandleRead()
	{
		uint64_t rdata;
		int n = ::read(m_wakeup_fd, &rdate, sizeof(rdata));
		assert(n == sizeof(rdata);
		return true;
	}
}
