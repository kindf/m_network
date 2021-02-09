
#include "MysqlConnectionPool.h"
#include "MysqlConnection.h"

MysqlConnectionPool::MysqlConnectionPool(int init_num)
: m_init_num(init_num) 
{

}

MysqlConnectionPool::~MysqlConnectionPool()
{
    Release();
}

MysqlConnectionPool& MysqlConnectionPool::Instance()
{
    static MysqlConnectionPool pool;
    return pool;
}

void MysqlConnectionPool::Init(const char *dbhost, unsigned int dbport, const char *dbpw, const char *dbname, const char *dbuser, const char* charset)
{
    for(int i = 0; i < m_init_num; ++i)
    {
        MysqlConnection* conn = new MysqlConnection(dbhost, dbport, dbpw, dbname,  dbuser, charset);
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

void MysqlConnectionPool::ReturnConn(MysqlConnection* conn)
{
    m_mutex.lock();
	m_conn_pool.push(conn);
	m_mutex.unlock();
}

MysqlConnection* MysqlConnectionPool::GetConn()
{
    if(m_conn_pool.empty())
    {
        return 0;
    }
    MysqlConnection* conn = 0;
    m_mutex.lock();
    conn = m_conn_pool.front();
    m_conn_pool.pop();
    m_mutex.unlock();
    if(conn == 0)
    {
        return 0;
    }
    if (!conn->IsActive())
	{
		conn->ReConnect();
		if (!conn->IsActive())
		{
            return 0;
        }
    }
    return conn;
}