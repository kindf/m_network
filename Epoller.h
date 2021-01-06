#ifndef EPOLLER__H__
#define EPOLLER__H__

#include <sys/epoll.h>
#include <map>
#include "Channel.h"
#include "EventLoop.h"


using namespace network;

namespace network
{
	//class Eventloop;
	//class Channel;

	class Epoller
	{
	public:
		Epoller(EventLoop* loop);
		~Epoller(){};
		
		void AssertInLoopThread();

		void Poll(int timeout, std::vector<Channel*> &active_channel);
		bool UpdateChannel(Channel* channel);
		void RemoveChannel(Channel* channel);
		
		typedef std::map<int, Channel*> ChannelPtrMap;
		typedef std::vector<struct epoll_event> EventVec;

		const int kNew = -1;
		const int kAdded = 1;
		const int kDeleted = 2;
	private:
		bool Update(int operaton, Channel* channel);
	private:
		static const int InitEventVecSize  = 16;

		int 			m_epollfd;
		ChannelPtrMap 	m_channel_map;	
		EventLoop* 		m_loop;					
		EventVec		m_event_vec; 
	};

}




#endif
