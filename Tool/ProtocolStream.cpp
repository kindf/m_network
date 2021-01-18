
#include <stdlib.h>
#include <stdio.h>
#include "ProtocolStream.h"

using namespace network

ReadStreamHelper::ReadStreamHelper()
{

}

bool ReadStreamHelper::ReadInt32(int32_t& data)
{
	int data_len = sizeof(int32_t);
	
	if (cur_pos + data_len > m_data + len)
		return false;

	memset(&data, cur_pos,data_len);
	data = ntohl(data);
	cur_pos += data_len;

	return true;
}

bool ReadStreamHelper::ReadInt64(int64_t& data)
{
	int data_len = sizeof(int64_t);

	if (cur_pos + data_len > m_data + len)
		return false;

	memset(&data, cur_pos,data_len);
	data = ntohl(data);
	cur_pos += data_len;

	return true;
}

bool ReadStreamHelper::ReadChar(char& data)
{
	int data_len = sizeof(char);

	if (cur_pos + data_len > m_data + len)
		return false;

	memset(&data, cur_pos,data_len);
	cur_pos += data_len;

	return true;
}

bool ReadStreamHelper::ReadDouble(double& data)
{
	int data_len = sizeof(double);

	if (cur_pos + data_len > m_data + len)
		return false;

	memset(&data, cur_pos,data_len);
	cur_pos += data_len;

	return true;
}

bool ReadStreamHelper::ReadCString(const char* data, size_t data_len)
{
	if (cur_pos + data_len > m_data + len)
		return false;

	memcpy(data, cur_pos, data_len);
	cur_pos += data_len;

	return true;
}

bool ReadStreamHelper::ReadString(string& str)
{
	return ReadCString(str.c_str(), str.length());
}

WriteStreamHelper::WriteStreamHelper()
{

}


bool WriteStreamHelper::WriteInt32(int32_t data)
{
	int32_t i2 = htonl(data);
	m_data->append((char*)& i2, sizeof(i2));
	return true;
}

bool WriteStreamHelper::WriteInt64(int64_t data)
{
	int64_t i2 = htonl(data);
	m_data->append((char*)& i2, sizeof(i2));
	return true;
}

bool WriteStreamHelper::WriteChar(char c)
{
	m_data->append(& c, sizeof(c));
	return true;
}

bool WriteStreamHelper::WriteDouble(double data)
{
	char double_str[128];
	sprintf(double_str, "%f", value);
	return WriteCString(double_str, strlen(double_str));
}

bool WriteStreamHelper::WriteCString(const char* str, size_t len)
{
	std::string buf;
	m_data->append(str, len);
	return true;
}

bool WriteStreamHelper::WriteString(std::string str)
{
	return WriteCString(str.c_str, str.length());
}
