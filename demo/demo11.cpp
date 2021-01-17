#include <iostream>
#include <string>
#include <functional>

#include "../TcpServer.h"
#include "../InetAddress.h"
#include "../EventLoop.h"
#include "../Callback.h"
#include "../TcpConnection.h"

void MessageFunc(const network::TcpConnectionPtr& conn_ptr)
{
	std::string msg = conn_ptr->GetInputStr();
	std::cout<<msg<<std::endl;
	conn_ptr->Send((const void*)msg.c_str(), msg.length());
	conn_ptr->ClearInputBuff();
}

int main(int agrc, char* agrv[])
{
	if(agrc < 2)
	{
		std::cout<<"agrv number error!!!"<<std::endl;
	}
	int port = atoi(agrv[1]);
	network::EventLoop main_loop;
	network::InetAddress addr(port);
	network::TcpServer m_server(&main_loop, addr, "Http Server");
	network::MessageCallback  msg_cb(std::bind(&MessageFunc, std::placeholders::_1));	
	m_server.SetMessageCallback(msg_cb);
	m_server.Start();
	main_loop.Loop();
	return 0;
}
