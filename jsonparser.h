
#ifndef JSON_CONFIG_PARSE_H
#define JSON_CONFIG_PARSE_H

#include "servercommon/turnfightcommon/squads.h"
/*
	一个简单的json配置读取工具
	适用于已知json格式读取的场景

	对于json_object和json_array对象，则down进入，读取完数据后up一次，支持栈式down和up
	对于其他类型则直接GetXXX即可
*/

struct json_object;

class JsonParser
{
public:
	JsonParser();
	~JsonParser();

	static const char *EMPTY_OBJ;
	static const char *EMPTY_ARRAY;

	bool Init(const char *str);
	bool InitFromFile(const char *filename);

	int GetListLength();

	bool Down(const char *node_str);
	bool Down(int index);
	void Up();

	bool ObjGetBoolean(const char *key, bool default_value);
	int ObjGetInt(const char *key, int default_value);
	double ObjGetDouble(const char *key, double default_value);
	const char *ObjGetString(const char *key, const char *default_value);

	bool ObjSetBoolean(const char *key, bool value);
	bool ObjSetInt(const char *key, int value);
	bool ObjSetDouble(const char *key, double value);
	bool ObjSetString(const char *key, const char *value);
	bool ObjSetObjAndDown(const char *key);
	bool ObjSetArrayAndDown(const char *key);

	bool ArrayGetBoolean(int index, bool default_value);
	int ArrayGetInt(int index, int default_value);
	double ArrayGetDouble(int index, double default_value);
	const char *ArrayGetString(int index, const char *default_value);

	bool ArraySetBoolean(int index, bool value);
	bool ArraySetInt(int index, int value);
	bool ArraySetDouble(int index, double value);
	bool ArraySetString(int index, const char *value);
	bool ArraySetObjAndDown(int index);
	bool ArraySetArrayAndDown(int index);

	bool ArrayPushBackBoolean(bool value);
	bool ArrayPushBackInt(int value);
	bool ArrayPushBackDouble(double value);
	bool ArrayPushBackString(const char *value);
	bool ArrayPushBackObjAndDown();
	bool ArrayPushBackArrayAndDown();

	bool ArrayPushBackMsgSquads(MsgSquads *msg_squads);
	bool ArrayGetMsgSquads(int key_index, MsgSquads &msg_squads);

	bool SaveInFile(const char *filename);

	const char *ToJsonString();
private:
	json_object *m_json_root;

	static const int MAX_PATH_DEEPT = 128;
	json_object *m_cur_node_path[MAX_PATH_DEEPT];
	int m_cur_path_deept;

	json_object *GetKeyHelper(const char *key);
	json_object *GetKeyHelper(int index);
};





#endif


