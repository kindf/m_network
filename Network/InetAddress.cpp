#include "InetAddress.h"
#include <string.h>

namespace network
{
	InetAddress::InetAddress(unsigned short port)
	{
		bzero(&m_addr, sizeof(m_addr));
		m_addr.sin_family = PF_INET;
		m_addr.sin_port = htons(port);
		m_addr.sin_addr.s_addr = INADDR_ANY;
	}

	InetAddress::InetAddress(const char* ip, unsigned short port)
	{
		bzero(&m_addr, sizeof(m_addr));
		m_addr.sin_family = PF_INET;
		m_addr.sin_port = htons(port);
		inet_pton(AF_INET, ip, &m_addr.sin_addr);
	}	

	InetAddress::InetAddress(const std::string& ip, unsigned short port)
	{
		bzero(&m_addr, sizeof(m_addr));
		m_addr.sin_family = PF_INET;
		m_addr.sin_port = htons(port);
		inet_pton(AF_INET, ip.c_str(), &m_addr.sin_addr);
	}	

	void InetAddress::SetSockAddrInet(struct sockaddr_in addr)
	{
		m_addr = addr;
	}

	// const string InetAddress::ToIpPort()
	// {
		
	// }
}
