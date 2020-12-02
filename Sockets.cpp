#include "Sockets.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <arpa/inet.h>


namespace network
{
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

	void Sockets::BindOrDie(const InetAddress* addr)
	{
		int ret = ::bind(sockfd_, addr->GetInetAddr(), sizeof(addr->GetInetAddr());
		assert(ret != -1);
	}

	void Sockets::ListenOrDie()
	{
		int ret = ::listen(sockfd_, SOMAXCONN);
		assert(ret >= 0);
	}	

	int Sockets::AcceptSock(InetAddress& peer_addr)
	{
		struct sockaddr_in addr;
    	memset(&addr, 0, sizeof addr);
		int connfd = Accept(m_sockfd, &addr);
		if (connfd >= 0)
		{
			peer_addr->SetSockAddrInet(addr);
		}
		return connfd;
	}

	int Sockets::CreateSocketOrDie()
	{
		int sockfd = ::socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
		assert(sockfd >= 0);
		return sockfd;
	}

	int Sockets::Accept(int fd, struct sockaddr_in* addr)
	{
		int connfd = ::accept(sockfd_, addr, static_cast<socklen_t>(sizeof(*addr));
		return connfd;
	}

	void Sockets::Close(int fd)
	{
		if (::close(fd) < 0)
		{
			std::cout<<"Sockets::Close() error! fd="<<fd<<std::endl;
		}
	}
}



	 
