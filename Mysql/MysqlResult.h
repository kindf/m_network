
#ifndef __MYSQL__RESULT__H
#define __MYSQL__RETURL__H

#include <map>
#include <vector>

#include "mysql/mysql.h"


class MysqlResult
{
public:
    MysqlResult();
    ~MysqlResult();

    bool Init(MYSQL* mysql);

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
	MYSQL_RES*		                    m_reuslt;
    std::map<std::string, IndexType>    m_field_name_info;          // key:列名
	std::vector<TypeName>		        m_field_index_info;                          
	unsigned int	                    m_field_count;
};


#endif



