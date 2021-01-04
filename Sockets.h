#ifndef __SOCKET__H
#define __SOCKET__H

namespace network
{
	class InetAddress;

	class Socket
	{
	public:
		explicit Sockets(int sockfd):m_sockfd(sockfd){}
		~Sockets();

	public:
		static int CreateSocketOrDie();
		int GetFd(){return m_sockfd;}
		void BindOrDie(const InetAddress* addr);
		void BindOrDie(const char* ip, short port);
		void ListenOrDie();
		int AcceptSock(InetAddress& peer_addr);
		static void Close(int fd);
	public:
		static int Accept(int fd, struct sockaddr_in* addr);
	private:
		int m_sockfd;
	}
}


#endif
