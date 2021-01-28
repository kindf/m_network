
#include <string>
#include <stdio.h>
#include <iostream>

#include "MysqlConnection.h"

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

	for(int i = 2; i < 10; ++i)
	{
		//string sql = "insert into test1 values (" + (char)('0'+i) + ", " + (char)('0'+i) + ");";
		char sql[100];
		sprintf(sql, "insert into test1 values (%d, %d)", i, i);
		if(!conn.Query(sql))
		{
			std::cout<<conn.GetError()<<std::endl;
		}
	}
	return 0;
}
