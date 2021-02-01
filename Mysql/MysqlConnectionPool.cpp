
#include "MysqlConnectionPool.h"
#include "MysqlConnection.h"

MysqlConnectionPool::MysqlConnectionPool(const char *dbhost, unsigned int dbport, const char *dbpw, const char *dbname, const char *dbuser, const char* charset)
: m_host(dbhost), m_port(dbport), m_pw(dbpw), m_database(dbname), m_username(dbuser), m_charset(charset), m_conn_pool()
{

}

MysqlConnectionPool::~MysqlConnectionPool()
{
    Release();
}

void MysqlConnectionPool::Init(int num)
{
    for(int i = 0; i < num; ++i)
    {
        MysqlConnection* conn = new MysqlConnection(m_host.c_str(), m_port, m_pw.c_str(), m_database.c_str(), m_username.c_str(), m_charset.c_str());
        if(conn->Connect())
        {
            m_conn_pool.push(conn);
        }
    }
}

void MysqlConnectionPool::Release()
{
    while (m_conn_pool.size() != 0)
	{
		delete m_conn_pool.front();
		m_conn_pool.pop();
	}
}

void MysqlConnectionPool::ReturnConn()
{
    m_mutex.Lock();
	m_conn_pool.push(conn);
	m_mutex.Unlock();
}

MysqlConnection* MysqlConnectionPool::GetConn()
{
    MysqlConnection* conn = 0;
    m_mutex.lock();
    if (m_conn_pool.size() != 0)
	{
		conn = m_conn_pool.front();
		m_conn_pool.pop();
	}
    m_mutex.unlock();
    if (!conn->IsActive())
	{
		conn->Reconnect();
		if (!conn->IsActive())
		{
            return 0;
        }
    }
    return conn;
}