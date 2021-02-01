
#include <iostream>
#include <cstring>
#include <sstream>

#include "HttpSession.h"
#include "../Network/TcpConnection.h"
#include "../Tool/StringTool.h"

using namespace network;

const char* ok_200_title = "OK";

const char* error_400_title = "Bad Request";
const char* error_400_form = "Your request has bad syntax ot is inherently impossible to satisfy.\n";

const char* error_403_title = "Forbidden";
const char* error_403_form = "You do not have permission to get file from this server.\n";

const char* error_404_title = "Not Found";
const char* error_404_form ="The requested file was not found on this server.\n";

const char* error_500_title = "Internal Error";
const char* error_500_form = "There was an unusual problem serving the requested file.\n";

namespace network
{

	HttpSession::HttpSession(std::shared_ptr<TcpConnection>& conn) 
	: m_temp_conn(conn)
	, m_http_response()
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
		const std::string& msg = conn_ptr->GetInputStr();
		std::cout<<msg<<std::endl;
		HTTP_CODE ret = ParseMsg(msg);
		// AddStatusLine(500, error_500_title);
		// AddHeader(strlen(error_500_form));
		// AddContent(error_500_form);
		// conn_ptr->Send((const void*)m_http_response.c_str(), m_http_response.length());
		// conn_ptr->ClearInputBuff();
	}

	HttpSession::HTTP_CODE HttpSession::ParseMsg(const std::string& msg)
	{
		string end = msg.substr(msg.length() - 4);
    	if (end != "\r\n\r\n")
        {
			return NO_REQUEST;
		}
		std::vector<string> lines;
    	StringTool::Split(msg, lines, "\r\n");
    	if (lines.empty() || lines[0].empty())
    	{
        	// conn->forceClose();
        	return NO_REQUEST;
    	}
		std::vector<string> chunk;
		StringTool::Split(lines[0], chunk, " ");
		if (chunk.size() < 3)
		{
			// conn->forceClose();
			return NO_REQUEST;
		}

		std::vector<string> part;
		StringTool::Split(chunk[1], part, "?");
		if (part.size() < 2)
		{
			// conn->forceClose();
			return NO_REQUEST;
		}
		std::cout<<part[0]<<std::endl<<part[1]<<std::endl;
		return GET_REQUEST;
	}

	void HttpSession::AddHeader(int content_len)
	{
		AddContentLength(content_len);
		AddLinger();
		AddBlankLine();
	}

	void HttpSession::AddContentLength(int content_len)
	{
		char content_line[128];
		sprintf(content_line, "Content-Length: %d\r\n", content_len);
		m_http_response.append(content_line);
	}

	void HttpSession::AddLinger()
	{
		m_http_response.append("Connection close\r\n");
	}

	void HttpSession::AddStatusLine(int status, const char* title)
	{
		char state_line[128];
		sprintf(state_line, "HTTP/1.1, %d %s\r\n", status, title);
		m_http_response.append(state_line);
	}

	void HttpSession::AddBlankLine()
	{
		m_http_response.append("\r\n");
	}

	void HttpSession::AddContent(const char* content)
	{
		m_http_response.append(content);
	}
}