
#include <iostream>
#include <cstring>
#include <sstream>

#include "HttpSession.h"
#include "../TcpConnection.h"

using namespace network;

namespace network
{

	HttpSession::HttpSession(std::shared_ptr<TcpConnection>& conn) : m_temp_conn(conn)
	{

	}

	TcpConnectionPtr HttpSession::GetConnectionPtr()
	{
		if (m_temp_conn.expired())
		{
			return NULL;
		}
		return m_temp_conn.lock();
	}

	void HttpSession::OnRead(const TcpConnectionPtr& conn_ptr)
	{
		std::string msg = conn_ptr->GetInputStr();
		std::cout<<msg<<std::endl;
		// std::string output_str();
		std::ostringstream os;
		std::string content("{\"code\": 0, \"msg\": ok}");
		os<<"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nDate: Wed, 16 May 2018 10:06:10 GMT\r\nContent-Length: "
			<< content.length() 
			<< "\r\n\r\n"
			<<content;
		conn_ptr->Send((const void*)os.str().c_str(), strlen(os.str().c_str()));
		conn_ptr->ClearInputBuff();
	}

	int HttpSession::ProcessRead()
	{
		// LINE_STATUS line_status = LINE_OK;
		// HTTP_CODE ret = NO_REQUEST;
		// char* text = 0;
		
		// while(((m_check_state == CHECK_STATE_CONTENT) && line_status == LINE_OK) || ((line_status = parse_line()) == LINE_OK ))
		// {
		// 	text = get_line();
		// 	m_start_line = m_checked_idx;
		// 	printf("get 1 http line: %s\n", text);

		// 	switch(m_check_state)
		// 	{
		// 		case CHECK_STATE_REQUESTLINE:
		// 		{
		// 			ret = parse_request_line(text);
		// 			if(ret == BAD_REQUEST)
		// 			{
		// 				return BAD_REQUEST;
		// 			}
		// 			break;
		// 		}
		// 		case CHECK_STATE_HEADER:
		// 		{
		// 			ret = parse_headers(text);
		// 			if(ret == BAD_REQUEST)
		// 			{
		// 				return BAD_REQUEST;
		// 			}
		// 			else if(ret == GET_REQUEST)
		// 			{
		// 				return do_request();
		// 			}
		// 			break;
		// 		}
		// 		case CHECK_STATE_CONTENT:
		// 		{
		// 			ret = parse_content(text);
		// 			if(ret == GET_REQUEST)
		// 			{
		// 				ret = parse_content(text);
		// 				if(ret == GET_REQUEST)
		// 				{
		// 					return do_request();
		// 				}
		// 				line_status =LINE_OPEN;
		// 				break;
		// 			}
		// 		}
		// 		default:
		// 		{
		// 			return INTERNAL_ERROR;
		// 		}
		// 	}
		// }
		return NO_REQUEST;
	}
}