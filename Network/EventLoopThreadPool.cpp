#include "EventLoopThreadPool.h"
#include "EventLoopThread.h"
#include "EventLoop.h"

using namespace network;

namespace network
{
    EventLoopThreadPool::EventLoopThreadPool(EventLoop* loop)
    : m_started(false)
    , m_loop(loop)
    , m_threads_num(0)
    , m_threads()
    , m_loops_list()
    , m_next_loop_idx(0)
    {

    }

    EventLoopThreadPool::~EventLoopThreadPool()
    {

    }

    void EventLoopThreadPool::Init(int threads_num, EventLoop* loop)
    {
        m_threads_num = threads_num;
        m_loop = loop;
    }

    void EventLoopThreadPool::Start(const ThreadInitCallback& cb)
    {
        if(m_loop == NULL)
        {
            return;
        }
        if(m_started)
        {
            return;
        }
        m_loop->AssertInLoopThread();
        for(int i = 0; i < m_threads_num; ++i)
        {
            std::unique_ptr<EventLoopThread> loop_ptr(new EventLoopThread(cb));
            m_loops_list.push_back(loop_ptr->StartLoop());
            m_threads.push_back(std::move(loop_ptr));
        }
        m_started = true;
    }

    void EventLoopThreadPool::Stop()
    {
        if(!m_started)
        {
            return;
        }
        for (auto& iter : m_threads)
        {
            iter->StopLoop();
        }
    }

    EventLoop* EventLoopThreadPool::GetNextLoop()
    {
        m_loop->AssertInLoopThread();
        if(!m_started)
        {
            return NULL;
        }
        EventLoop* loop = m_loop;
        if(!m_loops_list.empty())
        {
            loop = m_loops_list[m_next_loop_idx];
            m_next_loop_idx = (m_next_loop_idx+1)%m_loops_list.size();
        }
        return loop;
    }
}

