
#ifndef _MYSQL_CONNECTION_H
#define _MYSQL_CONNECTION_H

#include <string>

class MysqlConnection
{
public:
    MysqlConnection(const char *dbuser, const char *dbpw, const char *dbname, const char *dbhost, unsigned int dbport, const char* charset);
    ~MysqlConnection();

    bool Connect();
    bool ReConnect();
    bool IsActive()
    bool Query(const char* sql);

    void Close();
    int GetErrno()const;
    const char* GetError()const;
private:
    MYSQL*      m_mysql;

    string      m_username;
    string      m_pw;
    string      m_host;
    string      m_port;
    string      m_charset;
    string      m_database;

    bool        m_connected;

};

#endif