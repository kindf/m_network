
#include "TcpConnection.h"

namespace network
{
    TcpConnection::TcpConnection(EventLoop* loop, int fd)
    : m_loop = loop
    , m_channel_ptr(new Channel(loop, fd))
    , m_input_buff()
    , m_output_buff()
    {
        
    }

    void TcpConnection::HandleRead()
    {
        m_loop->AssertInLoopThread();
        int n = OnReadFd();

        if(n > 0)
        {
            m_message_callback(share_from_this());
        }
        else if(n == 0)
        {
            HandleClose();
        }
        else
        {
            HandleError();
        }
        
    }

    int TcpConnection::OnReadFd()
    {
        char buffer[MAX_INPUT_BUFFER_SIZE];
        int n = 0;
        while(1)
        {
            int read_byet = ::recv(m_channel.GetFd(), buffer, MAX_INPUT_BUFFER_SIZE-1, 0);
            if(read_byet > 0)
            { 
                n += read_byet;
            }   
            else if(read_tyet == 0)
            {
                HandleClose();
            }
            else
            {
                if(errno == EAGAIN)
                {
                    break;
                }
                HandleError();
            }
            
        };

        m_input_buff.append(buffer, n);
        return n;
    }

}