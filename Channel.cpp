#include "Channel.h"

using namespace network

Channel::Channel(const loop, const int fd)
:loop_(loop), fd_(fd), events_(0), revents_(0)
{
}

void Channel::SetReadEvent()
{
	events_ |= EPOLLIN;
	return Update();
}

void Channel::SetWriteEvent()
{
	events_ |= EPOLLOUT;
	return Update();
}

bool Channel::Update()
{
	return loop_->UpdateChannel(this);
}

void Channel::SetReadCallBack(const EventCallBack& cb)
{
	read_callback = cb;
}

void Channel::SetWriteCallBack(const EventCallBack& cb)
{
	write_callback = cb;
}

void Channel::SetErrorCallBack(const EventCallBack& cb)
{
	error_callback = cb;
}

