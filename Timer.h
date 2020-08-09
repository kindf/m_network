#ifndef __TIMER__H
#define __TIMER__H

#include <stdint.h>
#include <atomic>
#include "Timestamp.h"
#include "Callback.h"

namespace network
{
	class Timer
	{
	public:
		Timer(const TimerCallback& callback, Timestamp when, int64_t intercal, int64_t repeat_count = -1);
	
	void run();
	
	bool IsCanceled() const
	{
		return m_canceled
	}

	void Cancel(bool off)
	{
		m_canceled = off;
	}

	Timestamp Expiration()const {return m_expiration;}
	int64_t GetRepeatCount()const {return m_repeat_count;}
	int64_t Sequence()const {return m_sequence;}
	static int64_t NumCreated() {return m_created_num;}


	private:
		Timer(const Timer&) = delete;
		Timer operator =(const Timer&) = delete;
	private:
		const TimerCallback 	m_callback;
		Timestamp		m_expiration;
		int64_t 		m_interval;
		int64_t 		m_repeat_count;
		bool 			m_canceled;
		const int64_t		m_sequence;
		
		static std::atomic<int64_t> m_created_num;
	}


}
#endif
