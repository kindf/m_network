

#include <iostream>
#include "TcpConnection.h"
#include "EventLoop.h"

using namespace network;

namespace network
{
    TcpConnection::TcpConnection(EventLoop* loop, int fd, std::string name)
    : m_loop(loop)
    , m_channel_ptr(new Channel(loop, fd))
    , m_input_buff()
    , m_output_buff()
    , m_name(name)
    {
        m_channel_ptr->SetReadCallBack(std::bind(&TcpConnection::HandleRead, this));
        m_channel_ptr->SetWriteCallBack(std::bind(&TcpConnection::HandleWrite, this));
        // m_channel_ptr->SetErrorCallBack();
    }

    void TcpConnection::Send(const void* message, int len)
    {
        if(m_loop->IsInLoopThread())
        {
            m_loop->RunInLoop(std::bind(&TcpConnection::SendInLoop, this, message, len));
        }
        else
        {
            SendInLoop(message, len);
        }
        
    }

    void TcpConnection::SendInLoop(const void* data, int len)
    {
        m_loop->AssertInLoopThread();
        // FIXME
        m_channel_ptr->SetWriteEvent();
        m_output_buff.append((char*)data, len);
        HandleWrite();
    }

    void TcpConnection::HandleRead()
    {
        m_loop->AssertInLoopThread();
        int n = OnReadFd();

        if(n > 0)
        {
            m_message_callback(shared_from_this());
        }
        else if(n == 0)
        {
            // HandleClose();
        }
        else
        {
            // HandleError();
        }
        
    }

    void TcpConnection::HandleWrite()
    {
        m_loop->AssertInLoopThread();
        int total_len = m_output_buff.length();
        if(total_len == 0)
        {
            m_channel_ptr->DisableWriteEvent();
            return;
        }
        int n = send(m_channel_ptr->GetFd(), (const char*)m_output_buff.c_str(), total_len, 0);
        if(n > 0)
        {
            m_output_buff.erase(0, n);
            if(m_output_buff.length() == 0)
            {
                m_channel_ptr->DisableWriteEvent();
                // if(m_write_callback)
                // {
                //     m_loop->QueueInLoop(std::bind(m_write_callback, shared_from_this()));
                // }
            }
        }
        else
        {
            HandleClose();
        }
        
    }

    void TcpConnection::HandleClose()
    {
        m_loop->AssertInLoopThread();
        m_channel_ptr->DisableAllEvent();
        // m_close_callback(shared_from_this());
    }

    int TcpConnection::OnReadFd()
    {
        char buffer[MAX_INPUT_BUFFER_SIZE];
        int n = 0;
        while(1)
        {
            int read_byet = ::recv(m_channel_ptr->GetFd(), buffer, MAX_INPUT_BUFFER_SIZE-1, 0);
            if(read_byet > 0)
            { 
                n += read_byet;
            }   
            else if(read_byet == 0)
            {
                // HandleClose();
            }
            else
            {
                if(errno == EAGAIN)
                {
                    break;
                }
                // HandleError();
            }
            
        };

        m_input_buff.append(buffer, n);
        return n;
    }

    void TcpConnection::ConnectEstablished()
    {
        m_loop->AssertInLoopThread();
        std::cout<<"TcpConnection::ConnectEstablished"<<std::endl;
        if(!m_channel_ptr->SetReadEvent())
        {
            std::cout<<"TcpConnection::ConnectEstablished channel readevent error, connction close"<<std::endl;
            HandleClose();
        }

        m_connection_callback(shared_from_this());
    }

    void TcpConnection::ConnectDestroyed()
    {

    }
}