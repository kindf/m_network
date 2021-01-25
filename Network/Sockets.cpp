
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include "Sockets.h"
#include "InetAddress.h"

namespace network
{
	void Sockets::BindOrDie(const char* ip, short port)
	{
		struct sockaddr_in addr;
		bzero(&addr, sizeof(addr));
		addr.sin_family = PF_INET;
		addr.sin_port = htons(port);
		inet_pton(AF_INET, ip, &addr.sin_addr);
		
		int reuse = 1;
		setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
		int ret = ::bind(m_sockfd, (struct sockaddr*) &addr, sizeof(addr));
		if(ret == -1)
		{
			int error_num = -1;
			socklen_t len = sizeof(error_num);
			getsockopt(m_sockfd, SOL_SOCKET, SO_ERROR, &error_num, &len);
			std::cout<<"Bind error, error number:"<<error_num<<std::endl;
		}
		assert(ret != -1);
	}

	void Sockets::BindOrDie(const InetAddress* addr)
	{
		int reuse = 1;
		int opt_ret = setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
		std::cout<<"BindOrDie opt_ret:"<<opt_ret<<std::endl;
		int ret = ::bind(m_sockfd, (sockaddr*)&addr->GetInetAddr(), sizeof(addr->GetInetAddr()));
		assert(ret != -1);
	}

	void Sockets::ListenOrDie()
	{
		int ret = ::listen(m_sockfd, SOMAXCONN);
		assert(ret >= 0);
	}	

	int Sockets::AcceptSock(InetAddress& peer_addr)
	{
		struct sockaddr_in addr;
    	memset(&addr, 0, sizeof addr);
		int connfd = Accept(m_sockfd, &addr);
		if (connfd >= 0)
		{
			peer_addr.SetSockAddrInet(addr);
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
		socklen_t len = static_cast<socklen_t>(sizeof *addr);
		int connfd = ::accept(fd, (sockaddr*)addr, &len);
		SetSocketNonBlock(connfd);
		return connfd;
	}

	void Sockets::Close(int fd)
	{
		if (::close(fd) < 0)
		{
			std::cout<<"Sockets::Close() error! fd="<<fd<<std::endl;
		}
		// std::cout<<"Sockets::Close()! fd="<<fd<<std::endl;
	}

	void Sockets::SetSocketNonBlock(int fd)
	{
		int flags = ::fcntl(fd, F_GETFL, 0);
    	flags |= O_NONBLOCK;
    	int ret = ::fcntl(fd, F_SETFL, flags);
    	(void)ret;
	}
}



	 
