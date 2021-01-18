
#include <iostream>
#include <functional>

#include "HttpServer.h"
#include "HttpSession.h"
#include "../Network/EventLoop.h"
#include "../Network/TcpServer.h"
#include "../Network/InetAddress.h"
#include "../Network/TcpConnection.h"

using namespace network;

namespace network
{
    HttpServer::HttpServer()
    {

    }

    HttpServer::~HttpServer()
    {

    }

    bool HttpServer::Init(const char* ip, short port, EventLoop* loop)
    {
        InetAddress addr(ip, port);
        m_server.reset(new TcpServer(loop, addr, "Http Server"));
        m_server->SetConnectionCallback(std::bind(&HttpServer::OnConnected, this, std::placeholders::_1));
        m_server->Start();

        return true;
    }

    void HttpServer::UnInit()
    {
        m_session_list.clear();
        m_server->Stop();
    }

    void HttpServer::OnConnected(std::shared_ptr<TcpConnection> conn)
    {
        // if (conn->connected())
        // {
            std::shared_ptr<HttpSession> new_session(new HttpSession(conn));
            conn->SetMessageCallback(std::bind(&HttpSession::OnRead, new_session.get(), std::placeholders::_1));
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                m_session_list.push_back(new_session);
            }
        // }
        // else
        // {
        //     onDisconnected(conn);
        // }
    }

    void HttpServer::OnDisConnected(const std::shared_ptr<TcpConnection>& conn)
    {
            std::lock_guard<std::mutex> guard(m_mutex);
        for (auto iter = m_session_list.begin(); iter != m_session_list.end(); ++iter)
        {
            if ((*iter)->GetConnectionPtr() == NULL)
            {
                break;
            }

            if ((*iter)->GetConnectionPtr() == conn)
            {
                m_session_list.erase(iter);
                break;
            }
        }
    }
}



