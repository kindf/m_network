#ifndef __EVENTLOOP__H
#define __EVENTLOOP__H

#include <vector>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <stdint.h>
#include "Channel.h"
#include "Epoller.h"
#include "Callback.h"
#include "Timestamp.h"
#include "TimerId.h"
#include "TimerQueue.h"

using namespace network;

namespace network
{
	class Epoller;
	class TimerQueue;

	class EventLoop
	{
	public:
		typedef std::vector<Channel*> ChannelVec;
		typedef std::function<void()> Func;
	public:
		EventLoop();
		~EventLoop(){};
		void Loop();
		void Quit();
		bool IsInLoopThread()const {return m_tid == std::this_thread::get_id();}
		void AssertInLoopThread();
		void DoPendingFunc();
		bool UpdateChannel(Channel* channel);
	
		void RunInLoop(const Func& func);
		void QueueInLoop(const Func& func);

		TimerId RunAt(Timestamp& when, TimerCallback& cb);
		TimerId RunAfter(int64_t delay, TimerCallback& cb);
		TimerId RunEvery(int64_t interval, TimerCallback& cb);

		bool HandleRead();
	private:
		bool CreateWakeUpFd();
		bool WakeUp();

	private:
		bool 							m_looping;
		bool							m_quit;
		std::unique_ptr<Epoller>		m_epoller;
		ChannelVec 						m_channel_vec;
		const std::thread::id 			m_tid;
		int 							m_wakeup_fd;
		std::unique_ptr<Channel> 		m_wakeup_channel;
		std::vector<Func> 				m_pending_func;	
		Channel* 						m_cur_active_channel;
		std::unique_ptr<TimerQueue>		m_timer_queue;
		std::mutex						m_mutex;
		bool							m_calling_pending_func;
	};
}

#endif
