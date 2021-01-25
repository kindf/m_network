#ifndef __ACCEPTOR__H
#define __ACCEPTOR__H

#include "Callback.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"

using namespace network;

namespace network
{
    // class Channel;
    // class EventLoop;
    // class Sockets;
    // class InetAddress;

    class Acceptor
    {
    public:
        typedef std::function<void(int sockfd, const InetAddress&)> NewConnectionCallback;

    public:
        Acceptor(EventLoop* loop, const InetAddress* listent_addr);
        ~Acceptor();

        bool Listen();
        void SetNewConnectionCallback(const NewConnectionCallback& cb){m_new_connection_callback = cb;}
    private:
        Acceptor& operator=(const Acceptor&);
        Acceptor(const Acceptor&);
        void HandleRead();

    private:
        EventLoop*                      m_loop;
        Sockets                         m_accept_sock;
        Channel                         m_accept_channel;
        NewConnectionCallback           m_new_connection_callback;
        bool                            m_listening;
    };
}

#endif