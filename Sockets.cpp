#include "Sockets.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <arpa/inet.h>



using namespace network;

void Sockets::BindOrDie(const char* ip, short port)
{
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &addr.sin_addr);
	
	int ret = ::bind(sockfd_, (struct sockaddr*) &addr, sizeof(addr));
	assert(ret != -1);
}

void Sockets::BindOrDie(const InetAddress& addr)
{
	int ret = ::bind(sockfd_, addr.GetInetAddr(), sizeof(&addr.GetInetAddr());
	assert(ret != -1);
}

void Sockets::ListenOrDie()
{
	int ret = ::listen(sockfd_, SOMAXCONN);
	assert(ret >= 0);
}	

int Sockets::AccpetSoct(struct sockaddr_in* addr)
{
	int connfd = ::accept(sockfd_, addr, static_cast<socklen_t>(sizeof(*addr));
	return connfd;
}


static int Sockets::CreateTcpSocket()
{
	int sockfd = ::socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
	assert(sockfd >= 0);
	return sockfd;
}




	 
