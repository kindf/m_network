#ifndef __ACCEPTOR__H
#define __ACCEPTOR__H

#include "Callback.h"

namespace network
{
    class Channel;
    class EventLoop;
    class Socket;

    class Acceptor
    {
    public:
        typedef std::function<void(int sockfd, const InetAddress&)> NewConnectionCallback;

    public:
        Acceptor(EventLoop* loop);
        ~Acceptor();

        void Listen();
    private:
        Acceptor& operator=(const Acceptor&);
        Acceptor(const Acceptor&);

        void HandleRead();

    private:
        EventLoop*                      m_loop;
        Channel                         m_accpet_channel;
        Socket                          m_accept_sock;
        NewConnectionCallback           m_new_connection_callback;
        bool                            m_listening;
    }
}


#endif