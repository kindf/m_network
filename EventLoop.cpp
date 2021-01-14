
#include <sys/eventfd.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> 
#include <iostream>

#include "EventLoop.h"
#include "Timestamp.h"

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
		m_wakeup_channel.reset(new Channel(this, m_wakeup_fd));
		m_wakeup_channel->SetReadCallBack(std::bind(&EventLoop::HandleRead, this));
		m_wakeup_channel->SetReadEvent();	
	}


	void EventLoop::Loop()
	{
		assert(IsInLoopThread());
		while(!m_quit)
		{
			m_timer_queue->DoTimer();
			m_channel_vec.clear();
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

	void EventLoop::RunInLoop(const Func& func)
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

	void EventLoop::QueueInLoop(const Func& func)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_pending_func.push_back(func);
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

	TimerId EventLoop::RunAt(Timestamp& when, TimerCallback& cb)
	{
		return m_timer_queue->AddTimer(cb, when, 0, 1);	
	}

	TimerId EventLoop::RunAfter(int64_t delay, TimerCallback& cb)
	{
		Timestamp time(AddTime(Timestamp::now(), delay));
		return RunAt(time, cb);
	}

	TimerId EventLoop::RunEvery(int64_t interval, TimerCallback& cb)
	{
		Timestamp time(AddTime(Timestamp::now(), interval));
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
		if(n != sizeof(wdata))
		{
			std::cout<<"EventLoop::WakeUp error, error msg: "<<std::endl;
			std::cout<<strerror(errno)<<std::endl;
			return false;
		}
		return true;
	}

	bool EventLoop::HandleRead()
	{
		uint64_t rdata;
		int n = ::read(m_wakeup_fd, &rdata, sizeof(rdata));
		assert(n == sizeof(rdata));
		return true;
	}

	bool EventLoop::UpdateChannel(Channel* channel)
	{
		if(channel->OwnerLoop() != this)
		{
			return false;
		}
		AssertInLoopThread();
		return m_epoller->UpdateChannel(channel);
	}
}
