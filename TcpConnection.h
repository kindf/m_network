#ifndef __TCPCONNECTION__H
#define __TCPCONNECTION__H

#include <string>
#include <memory>
#include "Channel.h"
#include "Callback.h"

namespace network
{
	class Channel;
	class Eventloop;

	class TcpConnection:public std::enable_shared_from_this<TcpConnection>
	{
	public:
		TcpConnection(EventLoop loop, int fd);
		~TcpConnection();

	private:
		TcpConnection(const TcpConnection&);
		TcpConnection& operator =(const TcpConnection&);


	private:
		ConnectionCallback			m_connection_callback;
		CloseCallback				m_close_callback;
		WriteCompleteCallback		m_write_callback;


		EventLoop*					m_loop;
		std::unique_ptr<Channel>	m_channel_ptr;
		std::string					m_input_buff;
		std::string					m_output_buff;
	};
}

#endif