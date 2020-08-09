#ifndef __INET_ADDRESS__H
#define __INET_ADDRESS__H

#include <string>


namespace network
{

	class InetAddress
	{
	public:
		explicit InetAddress(uint16_t port = 0);
		InetAddress(const char* ip, uint16_t port);
		InetAddress(const std::string& ip, uint16_t port);
		InetAddress(const struct sockaddr_in& addr):addr_(addr){}
		const struct sockaddr_in& GetInetAddr() const {return addr_;}
	private:
		struct sockaddr_in addr_;
	}
}	





#endif
