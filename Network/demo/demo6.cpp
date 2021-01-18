#include <functional>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "../EventLoop.h"
#include "../Sockets.h"
#include "../Channel.h"
#include "../InetAddress.h"

void ConnectionCallback(network::Sockets &listen_sock)
{
	// std::cout<<"connection !!!"<<std::endl;
	network::InetAddress peer_addr;
    int conn_fd = listen_sock.AcceptSock(peer_addr);
	if(conn_fd > 0)
	{
		std::cout<<"accept_fd = "<<conn_fd<<std::endl;
	}
	else
	{
		std::cout<<"1";
	}
		
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout<<"argv error!!!"<<" now argv len:"<<argc<<std::endl;
		return 0;
	}
	int port = atoi(argv[1]);
	std::cout<<"port is "<<port<<std::endl;
	int fd = network::Sockets::CreateSocketOrDie();
	network::Sockets sock(fd);
	sock.BindOrDie("127.0.0.1", port);
	sock.ListenOrDie();
	network::EventLoop main_loop;
	network::Channel channel(&main_loop, sock.GetFd());
	channel.SetReadCallBack(std::bind(&ConnectionCallback, sock));
	channel.SetReadEvent();
	main_loop.Loop();
	return 0;
}
