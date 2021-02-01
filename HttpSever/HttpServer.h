
#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

#include <memory>
#include <mutex>
#include <list>

// using namespace network;

namespace network
{
    class TcpServer;
    class HttpSession;
    class EventLoop;
    class TcpConnection;

    class HttpServer
    {
        public:
            HttpServer();
            ~HttpServer();

            
            bool Init(const char* ip, short port, EventLoop* loop);
            void UnInit();

        private:
            void OnConnected(std::shared_ptr<TcpConnection> conn);
            void OnDisConnected(const std::shared_ptr<TcpConnection>& conn);


            std::unique_ptr<TcpServer>                  m_server;
            std::list<std::shared_ptr<HttpSession>>     m_session_list;
            std::mutex                                  m_mutex;
            // std::unique_ptr<MysqlConnectionPool>        m_database;
    };
}

#endif