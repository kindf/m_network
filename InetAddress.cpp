#include <"InetAddress.h>

namespace network
{
	InetAddress::InetAddress(uint16_t port)
	{
		bzero(m_addr, sizeof(m_addr));
		m_addr.sin_family = PF_INET;
		m_addr.sin_port = htons(port);
		m_addr.sin_addr.s_addr = INADDR_ANY;
	}

	InetAddress::InetAddress(const char* ip, uint16_t port)
	{
		bzero(m_addr, sizeof(m_addr));
		m_addr.sin_family = PF_INET;
		m_addr.sin_port = htons(port);
		inet_pton(AF_INET, ip, &m_addr.sin_addr);
	}	

	InetAddress::InetAddress(const std::string& ip, uint16_t port)
	{
		bzero(m_addr, sizeof(m_addr));
		m_addr.sin_family = PF_INET;
		m_addr.sin_port = htons(port);
		inet_pton(AF_INET, ip.c_str(), &m_addr.sin_addr);
	}	
}
