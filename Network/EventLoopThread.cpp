#include "EventLoopThread.h"
#include "EventLoop.h"

using namespace network;

namespace network
{
    EventLoopThread::EventLoopThread(const ThreadInitCallback& init_cb)
    : m_exiting(false)
    , m_loop(NULL)
    , m_callback(init_cb)
    {

    }

    EventLoopThread::~EventLoopThread()
    {
        m_exiting = true;
        if (m_loop != NULL)
        {
            m_loop->Quit();
            m_thread->join();
        }
    }

    EventLoop* EventLoopThread::StartLoop()
    {
        m_thread.reset(new std::thread(std::bind(&EventLoopThread::ThreadFunc, this)));
        {
		    std::unique_lock<std::mutex> lock(m_mutex);
            while (m_loop == NULL)
            {
                m_cond.wait(lock);
            }
        }
        return m_loop;
    }

    void EventLoopThread::StopLoop()
    {
        if (m_loop != NULL)
        {
            m_loop->Quit();
        }
        m_thread->join();
    }

    void EventLoopThread::ThreadFunc()
    {
        EventLoop loop;
        if(m_callback)
        {
            m_callback(&loop);
        }
        {
            std::unique_lock<std::mutex> lock(m_mutex);
		    m_loop = &loop;
		    m_cond.notify_all();
        }
        loop.Loop();
        m_loop = NULL;
    }
}

