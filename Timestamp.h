#ifndef __TIMESTAMP__H
#define __TIMESTAMP__H

#include <stdint.h>
#include <algorithm>
#include <string>
using namaspace std;

class Timestamp
{
public:
	Timestamp():m_micro_seconds(0){}

	explicit Timestamp(int64_t micro_seconds);
	
	Timestamp& operator +=(Timestamp lhs)
	{
		this->m_micro_seconds += lhs.m_micro_seconds;
		return *this;	
	}				 

	Timestamp& operator +=(int64_t lhs)
	{
		this->m_micro_seconds += lhs;
		return *this;
	}

	Timestamp& operator -=(Timestamp lhs)
	{
		this->m_micro_seconds -= lhs.m_micro_seconds;
		return *this;
	}

	Timestamp& operator -=(int64_t lhs)
	{
		this->m_micro_seconds -= lhs;
		return *this;
	}

	bool Valid() const {return m_micro_seconds > 0;}

	void swap(Timestamp& that)
	{
		std::swap(m_micro_seconds, that.m_micro_seconds);

	}
	
	int64_t GetMicroSeconds() const{return m_micro_seconds;}

	static Timestamp now();
	static Timestamp Invalid();

private:
	static const int MICROSECONDSPERSECOND = 1000*1000;
	
	int64_t m_micro_seconds;
};

inline bool operator <(const Timestamp& lhs, const Timestamp& rhs)
{
	return lhs.GetMicroSeconds() < rhs.GetMicroSeconds();
}

inline bool operator >(Timestamp lhs, Timestamp rhs)
{
	return rhs < lhs;
}

inline bool operator <=(Timestamp lhs, Timestamp rhs)
{
	return !(lhs > rhs);
}

inline bool operator >=(Timestamp lhs, Timestamp rhs)
{
	return !(lhs < rhs):
}

inline bool operator ==(Timestamp lhs, Timestamp rhs)
{
	return lhs.GetMicroSeconds() == rhs.GetMicroSeconds();
}

inline bool operator !=(Timestamp lhs, Timestamp rhs)
{
	return !(lhs == rhs);
}

inline double TimeDefference(Timestamp high, Timestamp low)
{
	int64_t diff = high.GetMicroSeconds() - low.GetMicroSeconds();
	return static_cast<double>(diff)/Timestamp::MICROSECONDSPERSECOND;
}

inline Timestamp AddTime(Timestamp timestamp, int64_t microseconds)
{
	return Timestamp(timestamp.GetMicroSeconds() + microseconds);
}

#endif
