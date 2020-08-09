#ifndef __EVENTLOOP__H
#define __EVENTLOOP__H

#include <vetor>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "Channel.h"
#include "Epoller.h"

namespace network
{
	class EventLoop
	{
	public:
		typedef std::vector<Channel*> ChannelVec;
		typedef std::function<woid()> Func;
	public:
		EventLoop();
		~EventLoop();
		void Loop();
		void Quit();
		bool IsInLoopThread()const {return m_pid == std::this_thread::get_id();}
		void DoPendingFunc();
	private:
		bool CreateWakeUpFd();
		bool WakeUp();
		void HandleRead();

	private:
		bool 				m_looping;
		bool				m_quit;

		std::unique_ptr<Epoller>	m_epoller;
		ChannelVec 			m_channel_vec;
		const std::thread::id 		m_tid;

		int 				m_wakeup_fd;
		std::unique_ptr<Channel> 	m_wakeup_channel;
		
		std::vector<Func> 		m_peding_func;	
		
		Channel* 			m_cur_active_channel;
		
}

#endif
