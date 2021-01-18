#ifndef __INET_ADDRESS__H
#define __INET_ADDRESS__H

#include <string>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace network
{
	class InetAddress
	{
	public:
		static const int SIMPLE_INT = 999;
		explicit InetAddress(unsigned short port = 0);
		// InetAddress(){}
		InetAddress(const char* ip, unsigned short port);
		InetAddress(const std::string& ip, unsigned short port);
		InetAddress(const struct sockaddr_in& addr):m_addr(addr){}
		const struct sockaddr_in& GetInetAddr() const {return m_addr;}
		void SetSockAddrInet(struct sockaddr_in addr);
		//const string ToIpPort();
		~InetAddress(){}
	private:
		struct sockaddr_in m_addr;
	};
}	

#endif
