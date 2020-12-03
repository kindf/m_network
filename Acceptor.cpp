#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include <iostream>

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
            m_accept_channel.SetReadCallBack(std::bind(Acceptor::HandleRead, this));
        }

    void Acceptor::Listen()
    {
        m_loop->IsInLoopThread();
        m_listening = true;
        m_accept_sock.Listen();
        m_accept_channel.SetReadEvent();
    }

    void Acceptor::HandleRead()
    {
        m_loop->AssertInLoopThread();
        InetAddress peer_addr;
        int conn_fd = m_accept_sock.AcceptSoct(peer_addr);
        if(conn_fd >= 0)
        {
            if (m_new_connection_callback)
            {
                m_new_connection_callback(connfd, peerAddr);
            }
            else
            {
                Sockets::Close(connfd);
            }
        }
        else
        {
            std::count<<"Acceptor::HandleRead() "<<"accept error"<<std::endl;
        }
        
    }
} 
