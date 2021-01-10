#include "Channel.h"
#include "EventLoop.h"
#include <sys/epoll.h>
#include "Epoller.h"

using namespace network;

namespace network
{
	Channel::Channel(EventLoop* loop, const int fd)
	: m_loop(loop), m_fd(fd), m_events(0), m_revents(0), m_index(Epoller::kNew)
	{
	}

	bool Channel::SetReadEvent()
	{
		m_events |= EPOLLIN;
		return Update();
	}

	bool Channel::SetWriteEvent()
	{
		m_events |= EPOLLOUT;
		return Update();
	}

	bool Channel::Update()
	{
		return m_loop->UpdateChannel(this);
	}

	void Channel::SetReadCallBack(const EventCallBack& cb)
	{
		m_read_callback = cb;
	}

	void Channel::SetWriteCallBack(const EventCallBack& cb)
	{
		m_write_callback = cb;
	}

	void Channel::SetErrorCallBack(const EventCallBack& cb)
	{
		m_error_callback = cb;
	}

	void Channel::HandleEvent()
	{

	}
}

