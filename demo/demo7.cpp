// Demo for file "Acceptor.h"

#include <iostream>
#include <functional>

#include "../EventLoop.h"
#include "../Acceptor.h"
#include "../InetAddress.h"

void NewConnectionCallback(int fd, const InetAddress& addr)
{
	std::cout<<"new connection fd = "<<fd<<std::endl;
}

int main(int agrc, char* agrv[])
{
	if(agrc < 2)
	{
		std::cout<<"agrc error!!!"<<std::endl;
		return 0;		
	}
	int port = atoi(agrv[1]);
	network::InetAddress listen_addr(port);
	network::EventLoop main_loop;	
	network::Acceptor acceptor(&main_loop, &listen_addr);
	acceptor.SetNewConnectionCallback(std::bind(&NewConnectionCallback,std::placeholders::_1,  std::placeholders::_2));
	if(!acceptor.Listen())
	{
		std::cout<<"acceptor listen error"<<std::endl;
		return 0;
	}
	main_loop.Loop();
}
