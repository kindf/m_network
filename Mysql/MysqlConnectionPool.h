
#ifndef _MYSQL_CONNECTION_POOL_H
#define _MYSQL_CONNECTION_POOL_H

#include <mutex>
#include <queue>

class MysqlConnection;

class MysqlConnectionPool
{
public:
    MysqlConnectionPool();
    ~MysqlConnectionPool();

    void Init(int num = 4);
    void Release();

    MysqlConnection* GetConn();
    void ReturnConn();
private:
    std::mutex m_mutex;
    std::queue<MysqlConnection*> m_conn_pool;
};

#endif