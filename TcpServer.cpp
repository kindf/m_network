#include "TcpServer.h"

using namespace network;

namespace network
{
    TcpServer::TcpServer(EventLoop* loop,const InetAddress& listen_addr,const std::string& name_arg)
    : m_loop(loop)
    , m_name(name_arg)
    , m_hostport("9999")
    , m_acceptor(new Acceptor(loop, &listen_addr))
    , m_eventLoop_thread_pool(new EventLoopThreadPool(loop))
    , m_started(false)
    {
        m_acceptor->SetNewConnectionCallback(std::bind(&TcpServer::NewConnection, this));
    }

    TcpServer::~TcpServer()
    {
        m_loop->AssertInLoopThread();

        Stop();
    }

    void TcpServer::Start(int threads_num = 4)
    {
        if(!m_started)
        {
            m_eventLoopThreadPool.Init(threads_num, m_loop);
            m_eventLoopThreadPool.Start(m_threadinit_callback());
            m_loop->RunInLoop(std::bind(&Accept::Listen, m_acceptor.get());
            m_started = true;
        }
    }

    void TcpServer::Stop()
    {
        if(!m_started)
        {
            return;
        }
        for(auto iter = m_connections_map.begin(); iter != m_connections_map.end(), iter++)
        {
            TcpConnectionPtr conn = it->second;
            it->second.Reset();
            conn->GetLoop()->runInLoop(std::bind(&TcpConnection::ConnectDestroyed, conn));
            conn.Reset();
        }
        m_eventLoop_thread_pool.Stop();
    }

    
}