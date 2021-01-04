// Demo for file "Timestamp.h"
#include "../Timestamp.h"
#include <iostream>

int main()
{
	network::Timestamp timer(9090);
	network::Timestamp now = network::Timestamp::now();
	std::cout<<(timer < now)<<std::endl;
	return 0;
}
