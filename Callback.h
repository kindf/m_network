#ifndef __CALLBACKS__H
#define __CALLBACKS__H

#include <functional>
#include <memory>
#include "Timestamp.h"

namespace network
{
	class TcpConnection;
	typedef std::shared_ptr<TcpConnection> 						TcpConnectionPtr;
	typedef std::function<void()> 								TimerCallback;
	typedef std::function<void(const TcpConnectionPtr&)> 		ConnectionCallback;
	typedef std::function<void(const TcpConnectionPtr&)> 		CloseCallback;
	typedef std::function<void(const TcpConnectionPtr&)>		WriteCompleteCallback;
    typedef std::function<void(const TcpConnectionPtr&)> 		MessageCallback;
}

#endif
