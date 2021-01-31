
#ifndef _MYSQL_CONNECTION_H
#define _MYSQL_CONNECTION_H

#include <string>

#include "mysql/mysql.h"
#include "MysqlResult.h"

using std::string;

class MysqlConnection
{
public:
    MysqlConnection(const char *dbuser, const char *dbpw, const char *dbname, const char *dbhost, unsigned int dbport, const char* charset);
    ~MysqlConnection();

    bool Connect();
    bool ReConnect();
    bool IsActive();
    void Close();
    int GetErrno()const;
    const char* GetError()const;

    bool Query(const char* sql);

    MysqlResult* GetResult();

    // int Get();
    // int Insert();
    // int Update();
    // int Remove();
    // int Find();
    
private:

private:
    MYSQL*              m_mysql;

    string              m_username;
    string              m_pw;
    string              m_host;
    unsigned int        m_port;
    string              m_charset;
    string              m_database;

    bool                m_connected;
    MysqlResult         m_result;
};

#endif