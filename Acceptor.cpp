#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"

namespace network
{
    Acceptor::Acceptor(EventLoop* loop, const InetAddress* listent_addr, )
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
        int conn_fd = m_accept_sock.AcceptSoct();
        if(conn_fd >= 0)
        {

        }
    }
} 
