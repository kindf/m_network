// Demo for file "EventLoop.h"
// g++ -g -o demo5 demo5.cpp ../EventLoop.cpp ../TimerQueue.cpp ../Timer.cpp ../Timestamp.cpp ../Sockets.cpp ../Epoller.cpp ../Channel.cpp ../InetAddress.cpp

#include "../EventLoop.h"
#include <iostream>
#include <functional>

class TimerTest
{
public:
	static void TimerTestFunc()
	{
		std::cout<<"Timer test: count = "<<m_count<<std::endl;
		m_count++;
	}

	static int m_count;
};

int TimerTest::m_count = 10;

int main()
{
	network::EventLoop main_loop;
	network::TimerCallback func(std::bind(&TimerTest::TimerTestFunc));
	main_loop.RunEvery(1000000, func);
	main_loop.Loop();
	return 0; 
}
