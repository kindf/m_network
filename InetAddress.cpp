#include <"InetAddress.h>




using namespace network

InetAddress::InetAddress(uint16_t port)
{
	bzero(addr_, sizeof(addr_));
	addr_.sin_family = PF_INET;
	addr_.sin_port = htons(port);
	addr_.sin_addr.s_addr = INADDR_ANY;
}

InetAddress::InetAddress(const char* ip, uint16_t port)
{
	bzero(addr_, sizeof(addr_));
	addr_.sin_family = PF_INET;
	addr_.sin_port = htons(port);
	inet_pton(AF_INET, ip, &addr_.sin_addr);
}	

InetAddress::InetAddress(const std::string& ip, uint16_t port)
{
	bzero(addr_, sizeof(addr_));
	addr_.sin_family = PF_INET;
	addr_.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr);
}	
