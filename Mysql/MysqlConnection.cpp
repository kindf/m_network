
#include <string.h>

#include "MysqlConnection.h"

MysqlConnection::MysqlConnection(const char *dbhost, unsigned int dbport, const char *dbpw, const char *dbname, const char *dbuser, const char* charset)
: m_mysql(0), m_host(dbhost), m_port(dbport), m_pw(dbpw), m_database(dbname), m_username(dbuser), m_charset(charset), m_connected(false), m_result()
{

}

MysqlConnection::~MysqlConnection()
{
    Close();
}

bool MysqlConnection::Connect()
{
    if(m_connected)
    {
        return true;
    }
	int ret = 0;
	m_mysql = mysql_init(0);
	unsigned long client_flag = 0;
	client_flag |= CLIENT_FOUND_ROWS;
	if (0 == mysql_real_connect(m_mysql, m_host.c_str(), m_username.c_str(), m_pw.c_str(), m_database.c_str(), m_port, 0, client_flag))
	{
		Close();
		return false;
	}
	ret = mysql_set_character_set(m_mysql, m_charset.c_str());
	m_connected = true;
	return true;
}

bool MysqlConnection::ReConnect()
{
    return Connect();
}

bool MysqlConnection::Query(const char* sql)
{
    if(!m_connected || 0 == m_mysql)
    {
        return false;
    }
    if(0 != mysql_real_query(m_mysql,sql,(unsigned long)strlen(sql)))
	{
		return false;
	}
    // m_result.Init(m_mysql);
    return true;
}

bool MysqlConnection::IsActive()
{
	if (0 != m_mysql && 0 == mysql_ping(m_mysql))
	{
		return true;
	}
	return false;
}

void MysqlConnection::Close()
{
    if (0 != m_mysql)
	{
		mysql_close(m_mysql);
		mysql_library_end();
		m_mysql = 0;
	}
	m_connected = false;
}

int MysqlConnection::GetErrno() const
{
	return mysql_errno(m_mysql);
}

const char* MysqlConnection::GetError() const
{
	return mysql_error(m_mysql);
}

MysqlResult* MysqlConnection::GetResult()
{
	m_result.Reset();
	if(!m_result.Init(m_mysql))
	{
		return 0;
	}
	return &m_result;
}