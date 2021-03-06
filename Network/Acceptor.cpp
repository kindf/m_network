
#include <iostream>

#include "Acceptor.h"
#include "Sockets.h"

namespace network
{
    Acceptor::Acceptor(EventLoop* loop, const InetAddress* listent_addr)
        : m_loop(loop)
        , m_accept_sock(Sockets::CreateSocketOrDie())
        , m_accept_channel(loop, m_accept_sock.GetFd())
        , m_listening(false)
        {
            // FIXME:reuseaddr, reuseport
            m_accept_sock.BindOrDie(listent_addr);
            m_accept_channel.SetReadCallBack(std::bind(&Acceptor::HandleRead, this));
        }

    Acceptor::~Acceptor()
    {
        Sockets::Close(m_accept_sock.GetFd());
    }

    bool Acceptor::Listen()
    {
        m_loop->IsInLoopThread();
        m_listening = true;
        m_accept_sock.ListenOrDie();
        return m_accept_channel.SetReadEvent();
    }

    void Acceptor::HandleRead()
    {
        m_loop->AssertInLoopThread();
        InetAddress peer_addr;
        int conn_fd = m_accept_sock.AcceptSock(peer_addr);
        if(conn_fd >= 0)
        {
            if (m_new_connection_callback)
            {
                m_new_connection_callback(conn_fd, peer_addr);
            }
            else
            {
                Sockets::Close(conn_fd);
            }
        }
        else
        {
            std::cout<<"Acceptor::HandleRead() "<<"accept error"<<std::endl;
        }
        
    }
} 
