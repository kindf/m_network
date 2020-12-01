#ifndef __CHANNEL__H
#define __CHANNEL__H

#include "Sockets.h"
#include <functional>

using namespace network

namespace network
{
	//只支持epoll
	class Channel
	{
	public:
		typedef function<void()> EventCallBack;
		Channel(const EventLoop* loop, int fd);
		~Channel(){};

		bool SetReadEvent();
		bool SetWriteEvent();
		bool SetErrorEvent();
		bool Update();
		void SetReadCallBack(const EventCallBack& cb);
		void SetWriteCallBack(const EventCallBack& cb);
		void SetErrorCallBack(const EventCallBack& cb);
		void SetRevents(int revt){m_revents = revt;};
		int GetFd()const{return m_fd;};

	private:
		EventCallBack 			m_read_callback;
		EventCallBack 			m_write_callback;
		EventCallBack 			m_error_callback;	
		int 					m_fd;
		EventLoop* 				m_loop;
		int 					m_events;
		int 					m_revents;
	}
}

#endif
