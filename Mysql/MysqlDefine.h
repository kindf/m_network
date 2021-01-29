
#ifndef __MYSQL__DEFINE__H
#define __MYSQL__DEFINE__H

enum DBDATATYPE
{
    TYPE_CHAR,
    TYPE_INT,
    TYPE_LONG64,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_TIME,
    TYPE_DATE,
    TYPE_TIMESTAMP
};

int GetDataType(int nSqlType);

#endif