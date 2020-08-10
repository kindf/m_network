
#include "TcpConnection.h"

using namespace network;

TcpConnection::TcpConnection(EventLoop* loop, int fd)
: m_loop = loop
, m_channel_ptr(new Channel(loop, fd))
, m_input_buff()
, m_output_buff()
{
	
}

