// Demo for file "TImer.h"

#include "../Timer.h"
#include "../Timestamp.h"
#include <iostream>

void TimerFunc()
{
	std::cout<<"TimeFunc working!!!"<<std::endl;
}

int main()
{
	network::TimerCallback timer_cb(std::bind(TimerFunc));
	network::Timer timer(timer_cb, network::Timestamp::now(), 1, 10);
	while(true)
	{
		network::Timestamp now = network::Timestamp::now();
		if(timer.Expiration() <= now)
		{
			timer.run();
		}
	}
	return 0;
}
