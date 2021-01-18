// Demo for file "TcpConnection.h"

#include <iostream>

#include "../EventLoop.h"
#include "../TcpConnection.h"

int main()
{		
	network::EventLoop main_loop;
	network::TcpConnection conn(&main_loop, 6, "conn");
	return 0;
}
