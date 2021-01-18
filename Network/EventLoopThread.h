#ifndef __EVENTLOOP_THREAD__H
#define __EVENTLOOP_THREAD__H

#include <mutex>
#include <condition_variable> 
#include <thread>
#include <string>
#include <functional>

namespace network
{
    class EventLoop;

    class EventLoopThread
    {
    public:
        typedef std::function<void(EventLoop*)> ThreadInitCallback;

    public:
        EventLoopThread(const ThreadInitCallback& init_cb);
        ~EventLoopThread();
        EventLoop* StartLoop();
        void StopLoop();

    private:
        EventLoopThread(const EventLoopThread&);
        EventLoopThread& operator= (const EventLoopThread&);

        void ThreadFunc();

    private:
        bool                            m_exiting;
        std::unique_ptr<std::thread>    m_thread;
		std::mutex                      m_mutex;
		std::condition_variable         m_cond;
        EventLoop*                      m_loop;
        ThreadInitCallback              m_callback;
    };
}


#endif