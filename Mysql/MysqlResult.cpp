

#include "MysqlResult.h"
#include "MysqlDefine.h"

MysqlResult::MysqlResult()
: m_reuslt(0), m_field_name_info(), m_field_index_info(0), m_field_count(0)
{

}

MysqlResult::~MysqlResult()
{

}

bool MysqlResult::Init(MYSQL* mysql)
{
    m_reuslt = mysql_store_result(mysql);
    if (0 == m_reuslt)
	{
		return false;
	}
    m_field_count = mysql_num_fields(m_metadata);
    if(m_field_count <= 0)
    {
        return true;
    }

    MYSQL_FIELD* fields = mysql_fetch_fields(m_metadata);   // 获取
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
    return true;
}

