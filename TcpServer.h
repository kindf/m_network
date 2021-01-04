#ifndef __TCP_SERVER__H
#define __TCP_SERVER__H

#include <functional>
#include "Callback.h"

namespace network
{
    class EventLoop;
    class InetAddress;

    class TcpServer
    {
    public:
        typedef std::function<void(EventLoop*)> ThreadInitCallback;
    public:
        TcpServer(EventLoop* loop,
			      const InetAddress& listen_addr,
			      const std::string& name_arg,
			      Option option = kReusePort);
		~TcpServer();

        EventLoop* GetLoop() const { return m_loop; }
        void SetThreadInitCallback(const ThreadInitCallback& cb){ m_threadinit_callback = cb; }
        void Star(int threads_num = 4);
        void Stop();
        void SetConnectionCallback(const ConnectionCallback& cb){ m_connection_callback = cb; }
        void SetMessageCallback(const MessageCallback& cb){ m_message_callback = cb; }
        void SetWriteCompleteCallback(const WriteCompleteCallback& cb){ m_writecomplete_Callback = cb; }
        void RemoveConnection(const TcpConnectionPtr& conn);
    private:
        void NewConnection(int sockfd, const InetAddress& peerAddr);
        void RemoveConnectionInLoop(const TcpConnectionPtr& conn);
        typedef std::map<string, TcpConnectionPtr> ConnectionMap;
    private:
        EventLoop*                                      m_loop;
		const string                                    m_hostport;
		const string                                    m_name;
		std::unique_ptr<Acceptor>                       m_acceptor;
        std::unique_ptr<EventLoopThreadPool>            m_eventLoop_thread_pool;
		ConnectionCallback                              m_connection_callback;
		MessageCallback                                 m_message_callback;
		WriteCompleteCallback                           m_writecomplete_Callback;
		ThreadInitCallback                              m_threadinit_callback;
		std::atomic<int>                                m_started;
		int                                             m_nextConnId;
		ConnectionMap                                   m_connections_map;
    };
}

#endif