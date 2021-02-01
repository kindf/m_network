
// g++ -o test demo.cpp MysqlConnection.cpp MysqlResult.cpp MysqlDefine.cpp -I /usr/lib64/mysql -lmysqlclient

#include <string>
#include <stdio.h>
#include <iostream>

#include "MysqlConnection.h"
#include "MysqlResult.h"
#include "MysqlConnectionPool.h"

using std::string;

const char* dbname = "test_db";
const char* dbaddress = "localhost";
const char* dbpassword = "123456";
const char* dbuser = "root";
const char* chatset = "utf8";
unsigned int port = 3306;

int main()
{
	MysqlConnectionPool conn_pool;
	conn_pool.Init(dbaddress, port, dbpassword, dbname, dbuser, chatset);
	MysqlConnection* conn = conn_pool.GetConn();
	if(!conn)
	{
		// std::cout<<conn.GetError()<<std::endl;
		return 0;
	}
	
	if(!conn->Query("select * from test1"))
	{
		std::cout<<"Mysql Query false, msg:"<<conn->GetError()<<std::endl;
	}
	MysqlResult* result = conn->GetResult();
	if(result == 0)
	{
		std::cout<<"result error"<<std::endl;
		return 0;
	}
	result->Printf();
	conn_pool.ReturnConn(conn);
	return 0;
}
