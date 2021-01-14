#ifndef __EVENTLOOP_THREAD_POOL__H
#define __EVENTLOOP_THREAD_POOL__H

#include <vector>
#include <functional>
#include <memory>
#include <string>

#include "Callback.h"

namespace network
{
    class EventLoopThread;
    class EventLoop;

    class EventLoopThreadPool
    {
    public:
        typedef std::function<void(EventLoop*)> ThreadInitCallback;
    public:
        EventLoopThreadPool(EventLoop* loop);
        ~EventLoopThreadPool();
        void Init(int threads_num, EventLoop* loop);
        void Start(const ThreadInitCallback& cb);
        void Stop();
        EventLoop* GetNextLoop();
    private:
        EventLoopThreadPool(const EventLoopThreadPool&);
        EventLoopThreadPool& operator= (const EventLoopThreadPool&);

    private:
        bool                                            m_started;
        EventLoop*                                      m_loop;
        int                                             m_threads_num;
        std::vector<std::unique_ptr<EventLoopThread>>   m_threads;
        std::vector<EventLoop*>                         m_loops_list;
        int                                             m_next_loop_idx;
    };
}


#endif
