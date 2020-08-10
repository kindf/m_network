
#ifndef __PROTOCOLSTREAM__H
#define  __PROTOCOLSTREAM__H

#include <string>
#include <stdint.h>

namespace network
{
	class ReadStreamHelper final
	{
	public:
		ReadStreamHelper(const char* );

		//¶Á²Ù×÷
		bool ReadInt32(int32_t& data);
		bool ReadInt64(int64_t& data);
		bool ReadChar(char& data);
		bool ReadDouble(double& data);
		bool ReadCString(const char* data, size_t data_len);
		bool ReadString(string& str);

	private:
		ReadStreamHelper(const ReadStreamHelper&) = delete;
		ReadStreamHelper& operator =(const ReadStreamHelper&) = delete;

	private:
		const char*				m_data;
		const size_t			len;
		const char*				cur_pos;
	};


	class WriteStreamHelper final
	{
	public:
		WriteStreamHelper();

		//Ð´²Ù×÷
		bool WriteInt32(int32_t data);
		bool WriteInt64(int64_t data);
		bool WriteChar(char c);
		bool WriteDouble(double data);
		bool WriteCString(const char* str, size_t len);
		bool WriteString(std::string str);

	private:
		WriteStreamHelper(const WriteStreamHelper&) = delete;
		WriteStreamHelper& operator =(const WriteStreamHelper&) = delete;

	private:
		std::string*				m_data;
	};
}


#endif