#ifndef __INET_ADDRESS__H
#define __INET_ADDRESS__H

#include <string>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace network
{
	class InetAddress
	{
	public:
		explicit InetAddress(uint16_t port = 0);
		InetAddress(const char* ip, uint16_t port);
		InetAddress(const std::string& ip, uint16_t port);
		InetAddress(const struct sockaddr_in& addr):m_addr(addr){}
		const struct sockaddr_in& GetInetAddr() const {return m_addr;}
		void SetSockAddrInet(struct sockaddr_in addr);
	private:
		struct sockaddr_in m_addr;
	}
}	

#endif
