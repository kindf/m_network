
#include <iostream>

#include "MysqlResult.h"
#include "MysqlDefine.h"

MysqlResult::MysqlResult()
: m_result(0), m_field_count(0), m_field_name_info(), m_field_index_info(), m_row_data()
{

}

MysqlResult::~MysqlResult()
{
    if(m_result != 0)
    {
        mysql_free_result(m_result);
    }
}

bool MysqlResult::Init(MYSQL* mysql)
{
    if(mysql == 0)
    {
        return false;
    }
    m_result = mysql_store_result(mysql);
    if (0 == m_result)
	{
		return false;
	}
    m_field_count = mysql_num_fields(m_result);

    MYSQL_FIELD* fields = mysql_fetch_fields(m_result);   // 获取
    for(int i = 0; i < m_field_count; ++i)
    {
        MYSQL_FIELD* field = &fields[i];
        TypeName type_name;
        static const unsigned long MAX_BLOB_LENGTH = 1000 * 1024; 
        if (field->length > MAX_BLOB_LENGTH)
        {
            field->length = MAX_BLOB_LENGTH;
        }
        DBDATATYPE type = (DBDATATYPE)GetDataType((int)field->type);
        m_field_name_info[field->name].type = type;
        m_field_name_info[field->name].index = i;
        type_name.type = (int)type;
        type_name.name = field->name;
        m_field_index_info.push_back(type_name);
    }
    // NextRow();
    return true;
}

void MysqlResult::Reset()
{
    if(m_result == 0)
    {
        return;
    }
    mysql_free_result(m_result);
    m_field_name_info.clear();
    m_field_index_info.clear();
    m_field_count = 0;
    m_result = 0;
}

bool MysqlResult::NextRow()
{
    if(m_result == 0)
    {
        return false;
    }
    MYSQL_ROW row = mysql_fetch_row(m_result);
    if(row == 0)
    {
        return false;
    }
    m_row_data.clear();
    int field_len = mysql_num_fields(m_result);
    for(int i = 0; i < field_len; ++i)
    {
        m_row_data.push_back(row[i]);
    }
    return true;
}

void MysqlResult::Printf()
{
    
    while(NextRow())
    {
        for(int i = 0; i < (int)m_field_index_info.size(); ++i)
        {
            std::cout<<m_field_index_info[i].name<<": "<<m_row_data[i]<<"   ";
        }
        std::cout<<std::endl;
    }
}

