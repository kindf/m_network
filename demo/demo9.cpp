#include <iostream>
#include <functional>

#include "../EventLoopThread.h"
#include "../Timestamp.h"
#include "../EventLoop.h"

void ThreadFunc(network::EventLoop* loop)
{
	std::cout<<"ThreadLoop init!!!"<<std::endl;
}

void ThreadLoopTimerFunc()
{
	std::cout<<"Thread loop timer!!!"<<std::endl;	
}

void MainLoopTimerFunc()
{
	std::cout<<"Main loop timer!!!"<<std::endl;
}

int main()
{
	network::EventLoop main_loop;
	network::EventLoopThread thread(std::bind(&ThreadFunc, &main_loop));
	network::EventLoop* thread_loop = thread.StartLoop();
	network::Timestamp now = network::Timestamp::now();
	network::TimerCallback thread_func(std::bind(&ThreadLoopTimerFunc));
	network::TimerCallback main_func(std::bind(&MainLoopTimerFunc));
	thread_loop->RunEvery(1000000, thread_func);
	main_loop.RunEvery(1000000, main_func);
	main_loop.Loop();
	return 0;		
}
