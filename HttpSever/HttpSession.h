#ifndef _HTTP_SESSION_H
#define _HTTP_SESSION_H

#include <string>
#include <memory>
#include <sys/types.h>    
#include <sys/stat.h>   
#include <sys/uio.h>

#include "../Network/Callback.h"

using namespace network;

namespace network
{
    class TcpConnection;

    class HttpSession
    {
    public:
        enum HTTP_CODE 
        { 
            NO_REQUEST,             // 请求不完整
            GET_REQUEST,            // 完整客户端请求
            BAD_REQUEST,            // 请求语法错误
            NO_RESOURCE,            // 没有资源
            FORBIDDEN_REQUEST,      // 禁止请求
            FILE_REQUEST,           // 请求文件
            INTERNAL_ERROR,         // 内部错误
            CLOSE_CONNECTION        // 客户关闭连接
        };

        enum CHECK_STATE
        {
            CHECK_STATE_REQUESTLINE = 0,        // 分析请求行
            CHECK_STATE_HEADER,                 // 分析请求头部
            CHECK_STATE_CONTENT                 // 分析请求内容
        };
        
        enum LINE_STATUS                // 行读取状态
        {
            LINE_OK = 0,                // 读到完整行
            LINE_BAD,                   // 行出错
            LINE_OPEN                   // 行数据不完整
        };

    public:
        HttpSession(std::shared_ptr<TcpConnection>& conn);
        TcpConnectionPtr GetConnectionPtr();
        void OnRead(const TcpConnectionPtr& conn_ptr);

    private:
        void AddHeader(int content_len);
        void AddContentLength(int content_len);
        void AddLinger();
        void AddStatusLine(int status, const char* title);
        void AddBlankLine();
        void AddContent(const char* content);
    private:
        std::weak_ptr<TcpConnection>        m_temp_conn;
        int                                 m_check_state;

        std::string         m_url;
        std::string         m_version;
        std::string         m_host;

        std::string         m_http_response;    // 相应内容

        struct stat m_file_stat;
        struct iovec m_iv[2];
    };
}

#endif
