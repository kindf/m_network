
// g++ -o test demo.cpp MysqlConnection.cpp MysqlResult.cpp MysqlDefine.cpp -I /usr/lib64/mysql -lmysqlclient

#include <string>
#include <stdio.h>
#include <iostream>

#include "MysqlConnection.h"
#include "MysqlResult.h"

using std::string;

const char* dbname = "test_db";
const char* dbaddress = "localhost";
const char* dbpassword = "123456";
const char* dbuser = "root";
const char* chatset = "utf8";
unsigned int port = 3306;

int main()
{
	MysqlConnection conn(dbuser, dbpassword, dbname, dbaddress, port, chatset);
	if(!conn.Connect())
	{
		std::cout<<conn.GetError()<<std::endl;
		return 0;
	}
	
	if(!conn.Query("select * from test1"))
	{
		std::cout<<"Mysql Query false, msg:"<<conn.GetError()<<std::endl;
	}
	MysqlResult* result = conn.GetResult();
	if(result == 0)
	{
		std::cout<<"result error"<<std::endl;
		return 0;
	}
	result->Printf();
	return 0;
}
