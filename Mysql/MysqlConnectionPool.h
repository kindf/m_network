
#ifndef _MYSQL_CONNECTION_POOL_H
#define _MYSQL_CONNECTION_POOL_H

#include <mutex>
#include <queue>

class MysqlConnection;

class MysqlConnectionPool
{
public:
    MysqlConnectionPool(const char *dbhost, unsigned int dbport, const char *dbpw, const char *dbname, const char *dbuser, const char* charset);
    ~MysqlConnectionPool();

    void Init(int num = 4);
    void Release();

    MysqlConnection* GetConn();
    void ReturnConn();
private:
    string                              m_host;
    unsigned int                        m_port;
    string                              m_pw;
    string                              m_database;
    string                              m_username;
    string                              m_charset;

    std::mutex                          m_mutex;
    std::queue<MysqlConnection*>        m_conn_pool;
};

#endif