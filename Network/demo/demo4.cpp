// Demo for file "Sockets.h"

#include "../Sockets.h"
#include "../InetAddress.h"
#include <iostream>

int main()
{
	int fd = network::Sockets::CreateSocketOrDie();
	network::Sockets sock(fd);
	network::InetAddress address(9999);
	sock.BindOrDie(&address);
	sock.ListenOrDie();
	std::cout<<sock.GetFd()<<std::endl;
	return 0;
}
