#ifndef __CALLBACKS__H
#define __CALLBACKS__H

#include <functional>
#include <memory>
#include "Timestamp.h"

namespace network
{
	class TcpConnection;
	typedef std::share_ptr<TcpConnection> 					TcpConnectionPtr;
	typedef std::function<void()> 							TimerCallback;
	typedef std::function<void(const TcpConnection&)> 		ConnectionCallback;
	typedef std::function<void(const TcpConnection&)> 		CloseCallback;
	typedef std::function<void(const TcpConnection&)>		WriteCompleteCallback;
    typedef std::function<void(const TcpConnectionPtr&)> 	MessageCallback;
}

#endif
