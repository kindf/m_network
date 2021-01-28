
#include "MysqlConnectionPool.h"
#include "MysqlConnection.h"

MysqlConnectionPool::MysqlConnectionPool()
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
        MysqlConnection* conn = new MysqlConnection();
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