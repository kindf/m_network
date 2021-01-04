#include "Timestamp.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdio.h>

namespace network
{
	Timestamp::Timestamp(int64_t micro_seconds):m_micro_seconds(micro_seconds)
	{
	}

	Timestamp Timestamp::now()
	{
		chrono::time_point<chrono::system_clock, chrono::microseconds> now = chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now());

		int64_t micro_seconds = now.time_since_epoch().count();
		Timestamp time(micro_seconds);
		return time;		
	}

	Timestamp Timestamp::Invalid()
	{
		return Timestamp();
	}
}

