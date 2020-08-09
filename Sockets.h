#ifndef __SOCKET__H
#define __SOCKET__H

namespace network
{
	class Socket
	{
	public:
		explicit Socket(int sockfd):sockfd_(sockfd){}
		~Socket();

	public:
		static int CreateSocket();
	private:
		int sockfd_;

	}
}


#endif
