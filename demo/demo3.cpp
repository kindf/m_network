// Demo for file "InetAddress.h"

#include "../InetAddress.h"
#include <iostream>
#include <stdlib.h>

int main()
{
	network::InetAddress addr(99);
	std::cout<<network::InetAddress::SIMPLE_INT<<std::endl;	
	return 0;		
}
