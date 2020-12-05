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

    void TcpServer::RemoveConntion(const TcpConnectionPtr& conn)
    {
        m_loop->RunInLoop(std::bind(&TcpServer::RemoveConnectionInLoop, this, conn));
    }

    void TcpServer::NewConnection(int sockfd, const InetAddress& peerAddr)
    {
        m_loop.AssertInLoopThread();
        EventLoop* io_loop = m_eventLoop_thread_pool.GetNextLoop();
        if(io_loop == NULL)
        {
            std::cout<<"TcpServer::NewConnection "<<"io_loop is NULL"<<std::endl;
            return;
        }

        char conn_name[32] = "TcpConnection_%d";
        strcpy(conn_name, sockfd);
        TcpConnectionPtr conn(new TcpConnection(io_loop, sockfd, conn_name));
        conn->SetConnectionCallback(m_connection_callback);
        conn->SetMessageCallback(m_message_callback);
        conn->SetWriteCompleteCallback(m_writecomplete_Callback);
        m_connections_map.insert(std::make_pairs(conn_name, conn));

        io_loop->RunInLoop(std::bind(&TcpConnection::ConnectEstablished, conn));
    }

    void TcpServer::RemoveConnectionInLoop(const TcpConnectionPtr& conn)
    {
        m_loop->assertInLoopThread();
        const string conn_name = conn->GetName():
        std::cout<<"TcpServer::RemoveConnectionInLoop "<<"conn_name"<<std::endl;
        size_t n = m_connections_map.erase(conn->name());
        if (n != 1)
        {
            //出现这种情况，是TcpConneaction对象在创建过程中，对方就断开连接了。
            std::cout<<"TcpServer::RemoveConnectionInLoop"<<"connection "<<conn_name.c_str()<<" connection does not exist."std::endl;
            return;
        }

        EventLoop* io_Loop = conn->GetLoop();
        io_Loop->QueueInLoop(std::bind(&TcpConnection::ConnectDestroyed, conn));
    }
}