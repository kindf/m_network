#include <assert.h>
#include "Epoller.h"
#include <cstring>

using namespace network;

namespace network
{
	Epoller::Epoller(EventLoop* loop):m_epollfd(-1), m_loop(loop), m_event_vec(InitEventVecSize)
	{	
		m_epollfd = epoll_create(5);
		assert(m_epollfd != -1);
		m_channel_map.clear();	
	}

	void Epoller::Poll(int timeout, std::vector<Channel*> &active_channel_vec)
	{
		int ret_num = ::epoll_wait(m_epollfd, &*m_event_vec.begin(), 
						static_cast<int>(m_event_vec.size()),
						timeout);
		if(ret_num > 0)
		{
			for(int i=0; i < ret_num; ++i)
			{
				Channel* channel = static_cast<Channel*>(m_event_vec[i].data.ptr);
				int fd = channel->GetFd();
				ChannelPtrMap::const_iterator it = m_channel_map.find(fd);
				if(it == m_channel_map.end() || it->second != channel)
				{
					continue;
				}
				channel->SetRevents(m_event_vec[i].events);
				active_channel_vec.push_back(channel);
			}	
			if(static_cast<int>(ret_num) == m_event_vec.size())
			{
				m_event_vec.resize(ret_num*2);
			}
		}
		else if(ret_num == 0)
		{
			
		}
		else
		{

		}
	}

	void Epoller::AssertInLoopThread()
	{
		m_loop->AssertInLoopThread();
	}

	bool Epoller::UpdateChannel(Channel* channel)
	{
		AssertInLoopThread();
		int fd = channel->GetFd();
		const int index = channel->GetIndex();
		if(index == kNew)
		{
			if(m_channel_map.find(fd) != m_channel_map.end())
			{
				//添加channel，不应该已经存在channel的，有问题。。
				return false;
			}
			m_channel_map[fd] = channel;	
		}	
		else if(index == kDeleted)
		{
			if(m_channel_map.find(fd) == m_channel_map.end())
			{
				//删除channel不应该找不到channel，有问题。。
				return false;
			}
			if(m_channel_map[fd] != channel)
			{
				return false;
			}
			channel->SetIndex(kAdded);
			return Update(EPOLL_CTL_ADD, channel);	
		}
		else
		{
			if(m_channel_map.find(fd) == m_channel_map.end() || m_channel_map[fd] != channel || index != kAdded)
			{
				return false;
			}
			if(channel->IsNoneEvent())
			{
				if(Update(EPOLL_CTL_DEL, channel))
				{
					channel->SetIndex(kDeleted);
					return true;
				}
				else
				{
					return Update(EPOLL_CTL_MOD, channel);
				}
			}
		}	
	}

	void Epoller::RemoveChannel(Channel* channel)
	{
		AssertInLoopThread();
		int fd = channel->GetFd();
		if(m_channel_map.find(fd) == m_channel_map.end() || m_channel_map[fd] != channel || channel->IsNoneEvent())
		{
			return;
		}

		int index = channel->GetIndex();
		if(index != kAdded && index != kDeleted)
		{
			return;
		}

		size_t n = m_channel_map.erase(fd);
		if(n != 1)
		{
			return;
		}
		
		if(index == kAdded)
		{
			Update(EPOLL_CTL_DEL, channel);
		}
		channel->SetIndex(kNew);	
	}

	bool Epoller::Update(int operation, Channel* channel)
	{
		struct epoll_event event;
		memset(&event, 0, sizeof(event));
		event.data.ptr = channel;
		event.events = channel->GetEvents();
		int fd = channel->GetFd();
		if(::epoll_ctl(m_epollfd, operation, fd, &event) < 0)
		{
			return false;
		}
		return true;
	}
}