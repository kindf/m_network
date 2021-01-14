
#include <iostream>

#include "../TcpServer.h"
#include "../EventLoop.h"
#include "../InetAddress.h"

int main()
{
	network::EventLoop main_loop;
	network::InetAddress addr(9989);
	network::TcpServer m_server(&main_loop, addr, "DemoServer");
	return 0;
}
