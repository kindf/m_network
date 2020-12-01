#ifndef __SOCKET__H
#define __SOCKET__H

namespace network
{
	class Socket
	{
	public:
		explicit Socket(int sockfd):m_sockfd(sockfd){}
		~Socket();

	public:
		static int CreateSocketOrDie();
		int GetFd(){return m_sockfd;}
		void BindOrDie(const InetAddress* addr);
		void BindOrDie(const char* ip, short port);
		void ListenOrDie();
		int AcceptSock(struct sockaddr_in* addr);

	public:
		static void Accpet(int fd, struct sockaddr_in* addr);
	private:
		int m_sockfd;

	}
}


#endif
