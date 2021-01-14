
#include <iostream>
#include <string>
#include <string.h>

#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpConnection.h"

using namespace network;

namespace network
{
    TcpServer::TcpServer(EventLoop* loop,const InetAddress& listen_addr,const std::string& name_arg)
    : m_loop(loop)
    , m_name(name_arg)
    , m_hostport("9999")
    , m_acceptor(new Acceptor(loop, &listen_addr))
    , m_eventloop_thread_pool(new EventLoopThreadPool(loop))
    , m_started(false)
    {
        m_acceptor->SetNewConnectionCallback(std::bind(&TcpServer::NewConnection, this, std::placeholders::_1, std::placeholders::_2));
    }

    TcpServer::~TcpServer()
    {
        m_loop->AssertInLoopThread();

        Stop();
    }

    void TcpServer::Start(int threads_num)
    {
        if(!m_started)
        {
            m_eventloop_thread_pool->Init(threads_num, m_loop);
            m_eventloop_thread_pool->Start(m_threadinit_callback);
            m_loop->RunInLoop(std::bind(&Acceptor::Listen, m_acceptor.get()));
            m_started = true;
        }
    }

    void TcpServer::Stop()
    {
        if(!m_started)
        {
            return;
        }
        for(auto iter = m_connections_map.begin(); iter != m_connections_map.end(); iter++)
        {
            TcpConnectionPtr conn = iter->second;
            iter->second.reset();
            conn->GetLoop()->RunInLoop(std::bind(&TcpConnection::ConnectDestroyed, conn));
            conn.reset();
        }
        m_eventloop_thread_pool->Stop();
    }

    void TcpServer::RemoveConnection(const TcpConnectionPtr& conn)
    {
        m_loop->RunInLoop(std::bind(&TcpServer::RemoveConnectionInLoop, this, conn));
    }

    void TcpServer::NewConnection(int sockfd, const InetAddress& peerAddr)
    {
        m_loop->AssertInLoopThread();
        EventLoop* io_loop = m_eventloop_thread_pool->GetNextLoop();
        if(io_loop == NULL)
        {
            std::cout<<"TcpServer::NewConnection "<<"io_loop is NULL"<<std::endl;
            return;
        }

        char conn_name[32];
        sprintf(conn_name, "TcpConnection_%d", sockfd);
        TcpConnectionPtr conn(new TcpConnection(io_loop, sockfd, conn_name));
        conn->SetConnectionCallback(m_connection_callback);
        conn->SetMessageCallback(m_message_callback);
        conn->SetWriteCompleteCallback(m_writecomplete_callback);
        m_connections_map.insert(std::make_pair(conn_name, conn));

        io_loop->RunInLoop(std::bind(&TcpConnection::ConnectEstablished, conn));
    }

    void TcpServer::RemoveConnectionInLoop(const TcpConnectionPtr& conn)
    {
        m_loop->AssertInLoopThread();
        const string conn_name = conn->GetName();
        std::cout<<"TcpServer::RemoveConnectionInLoop "<<"conn_name"<<std::endl;
        size_t n = m_connections_map.erase(conn->GetName());
        if (n != 1)
        {
            //出现这种情况，是TcpConneaction对象在创建过程中，对方就断开连接了。
            std::cout<<"TcpServer::RemoveConnectionInLoop"<<"connection "<<conn_name.c_str()<<" connection does not exist."<<std::endl;
            return;
        }

        EventLoop* io_Loop = conn->GetLoop();
        io_Loop->QueueInLoop(std::bind(&TcpConnection::ConnectDestroyed, conn));
    }
}