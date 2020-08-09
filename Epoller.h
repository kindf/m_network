#ifndef EPOLLER__H__
#define EPOLLER__H__

#include <sys/epoll.h>
#include <map>
#include "Channel.h"


using namespace network

namespace network
{
	class Eventloop;
	class Channel;

	class Epoller
	{
	public:
		Epoller();
		~Epoller();
		
		void AssertInLoopThread() const
		{
			m_loop->AssertInLoopThread();
		}

		void Poll(int timeout, std::vector<Channel> active_channel);
		bool UpdateChannel(Channel* channel);
		void RemoveChannel(Channel* channel);
		
	private:
		bool Update(int operaton, Channel* channel);
	private:
		typedef std::map<int, Channel*> ChannelMap;
		typedef std::vector<struct epoll_event> EventVec;
		static const int InitEventVecSize  = 16;

		int 		m_epollfd;
		ChannelMap 	m_channel_map;	
		EventLoop 	m_loop;					
		EventVec	m_event_vec; 
	} 

}




#endif
