
#include <iostream>

#include "HttpServer.h"
#include "../Network/EventLoop.h"

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout<<"argc error!!!"<<std::endl;
		return 0;
	}
	int port = atoi(argv[1]);
	network::EventLoop main_loop;
	network::HttpServer m_server;
	m_server.Init("127.0.0.1", port, &main_loop);
	main_loop.Loop();
}
