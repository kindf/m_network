
#ifndef _MYSQL_RESULT_H
#define _MYSQL_RESULT_H

#include <map>
#include <vector>
#include <string>

#include "mysql/mysql.h"


class MysqlResult
{
public:
    MysqlResult();
    ~MysqlResult();

    bool Init(MYSQL* mysql);
	void Reset();
	bool NextRow();
	
	void Printf();

    struct TypeName
	{
		int type;
		std::string name;
	};

    struct IndexType 
	{
		unsigned int index;
		int type;
	};

private:
	MYSQL_RES*		                    m_result;
	unsigned int	                    m_field_count;
    std::map<std::string, IndexType>    m_field_name_info;          // key:列名
	std::vector<TypeName>		        m_field_index_info;                          
	std::vector<std::string>			m_row_data;			
};


#endif



