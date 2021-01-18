
#include "jsonparser.h"
#include "json.h"

const char *JsonParser::EMPTY_OBJ = "{}";
const char *JsonParser::EMPTY_ARRAY = "[]";

JsonParser::JsonParser():m_json_root(0), m_cur_path_deept(0)
{
	
}

JsonParser::~JsonParser()
{
	if (m_json_root != 0)
	{
		json_object_put(m_json_root);
	}
}

bool JsonParser::Init(const char *json_str)
{
	json_object *jobj = json_tokener_parse(json_str);

	if (is_error(jobj)) return false;

	if (m_json_root != 0)
	{
		json_object_put(m_json_root);
	}
	m_json_root = jobj;

	m_cur_node_path[0] = m_json_root;
	m_cur_path_deept = 1;

	return true;
}

bool JsonParser::InitFromFile(const char *filename)
{
	json_object *jobj = json_object_from_file(filename);

	if (is_error(jobj)) return false;

	m_json_root = jobj;

	m_cur_node_path[0] = m_json_root;
	m_cur_path_deept = 1;
	return true;
}

int JsonParser::GetListLength()
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];

	if (json_object_get_type(cur_json_obj) != json_type_array) return 0;

	return json_object_array_length(cur_json_obj);
}

json_object *JsonParser::GetKeyHelper(const char *key)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (0 == cur_json_obj) return 0;
	if (json_object_get_type(cur_json_obj) != json_type_object) return 0;

	json_object *next_json_obj = json_object_object_get(cur_json_obj, key);
	return next_json_obj;
}

json_object *JsonParser::GetKeyHelper(int index)
{
	if (index < 0) return 0;

	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return 0;

	array_list *data_array = json_object_get_array(cur_json_obj);
	if (data_array == 0) return 0;

	json_object *data_key_obj = (json_object*)array_list_get_idx(data_array, index);
	return data_key_obj;
}

bool JsonParser::Down(const char *node_str)
{
	if (m_cur_path_deept >= MAX_PATH_DEEPT) return false;

	json_object *next_json_obj = GetKeyHelper(node_str);
	if (next_json_obj == 0) return false;

	if (json_object_get_type(next_json_obj) != json_type_object && json_object_get_type(next_json_obj) != json_type_array) return false;

	m_cur_node_path[m_cur_path_deept++] = next_json_obj;

	return true;
}

bool JsonParser::Down(int index)
{
	if (m_cur_path_deept >= MAX_PATH_DEEPT) return false;

	json_object *next_json_obj = GetKeyHelper(index);
	if (next_json_obj == 0) return false;

	if (json_object_get_type(next_json_obj) != json_type_object && json_object_get_type(next_json_obj) != json_type_array) return false;

	m_cur_node_path[m_cur_path_deept++] = next_json_obj;

	return true;
}

void JsonParser::Up()
{
	if (m_cur_path_deept > 1) --m_cur_path_deept;
}

bool JsonParser::ObjGetBoolean(const char *key, bool default_value)
{
	json_object *value = GetKeyHelper(key);
	if (value == 0) return default_value;

	json_type t = json_object_get_type(value);
	if (t != json_type_boolean && t != json_type_int && t != json_type_double) return default_value;

	return json_object_get_boolean(value) != 0;
}

int JsonParser::ObjGetInt(const char *key, int default_value)
{
	json_object *value = GetKeyHelper(key);
	if (value == 0) return default_value;

	json_type t = json_object_get_type(value);
	if (t != json_type_int && t != json_type_double) return default_value;

	return json_object_get_int(value);
}

double JsonParser::ObjGetDouble(const char *key, double default_value)
{
	json_object *value = GetKeyHelper(key);
	if (value == 0) return default_value;

	json_type t = json_object_get_type(value);
	if (t != json_type_int && t != json_type_double) return default_value;

	return json_object_get_double(value);
}

const char *JsonParser::ObjGetString(const char *key, const char *default_value)
{
	json_object *value = GetKeyHelper(key);
	if (value == 0) return default_value;

	json_type t = json_object_get_type(value);
	if (t != json_type_string) return default_value;

	const char *str = json_object_get_string(value);
	return str == 0 ? default_value : str;
}

bool JsonParser::ArrayGetBoolean(int index, bool default_value)
{
	json_object *value = GetKeyHelper(index);
	if (value == 0) return default_value;

	json_type t = json_object_get_type(value);
	if (t != json_type_boolean && t != json_type_int && t != json_type_double) return default_value;

	return json_object_get_boolean(value) != 0;
}

int JsonParser::ArrayGetInt(int index, int default_value)
{
	json_object *value = GetKeyHelper(index);
	if (value == 0) return default_value;

	json_type t = json_object_get_type(value);
	if (t != json_type_int && t != json_type_double) return default_value;

	return json_object_get_int(value);
}

double JsonParser::ArrayGetDouble(int index, double default_value)
{
	json_object *value = GetKeyHelper(index);
	if (value == 0) return default_value;

	json_type t = json_object_get_type(value);
	if (t != json_type_int && t != json_type_double) return default_value;

	return json_object_get_double(value);
}

const char *JsonParser::ArrayGetString(int index, const char *default_value)
{
	json_object *value = GetKeyHelper(index);
	if (value == 0) return default_value;

	json_type t = json_object_get_type(value);
	if (t != json_type_string) return default_value;

	const char *str = json_object_get_string(value);
	return str == 0 ? default_value : str;
}

bool JsonParser::ObjSetBoolean(const char *key, bool value)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_object) return false;

	json_object_object_add(cur_json_obj, key, json_object_new_boolean(value));
	return true;
}

bool JsonParser::ObjSetInt(const char *key, int value)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_object) return false;

	json_object_object_add(cur_json_obj, key, json_object_new_int(value));
	return true;
}

bool JsonParser::ObjSetDouble(const char *key, double value)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_object) return false;

	json_object_object_add(cur_json_obj, key, json_object_new_double(value));
	return true;
}

bool JsonParser::ObjSetString(const char *key, const char *value)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_object) return false;

	json_object_object_add(cur_json_obj, key, json_object_new_string(value));
	return true;
}

bool JsonParser::ObjSetObjAndDown(const char *key)
{
	if (m_cur_path_deept >= MAX_PATH_DEEPT) return false;

	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_object) return false;

	json_object *new_obj = json_object_new_object();
	json_object_object_add(cur_json_obj, key, new_obj);

	m_cur_node_path[m_cur_path_deept++] = new_obj;
	return true;
}

bool JsonParser::ObjSetArrayAndDown(const char *key)
{
	if (m_cur_path_deept >= MAX_PATH_DEEPT) return false;

	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_object) return false;

	json_object *new_array = json_object_new_array();
	json_object_object_add(cur_json_obj, key, new_array);

	m_cur_node_path[m_cur_path_deept++] = new_array;
	return true;
}

bool JsonParser::ArraySetBoolean(int index, bool value)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return false;

	json_object_array_put_idx(cur_json_obj, index, json_object_new_boolean(value));
	return true;
}

bool JsonParser::ArraySetInt(int index, int value)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return false;

	json_object_array_put_idx(cur_json_obj, index, json_object_new_int(value));
	return true;
}

bool JsonParser::ArraySetDouble(int index, double value)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return false;

	json_object_array_put_idx(cur_json_obj, index, json_object_new_double(value));
	return true;
}

bool JsonParser::ArraySetString(int index, const char *value)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return false;

	json_object_array_put_idx(cur_json_obj, index, json_object_new_string(value));
	return true;
}

bool JsonParser::ArraySetObjAndDown(int index)
{
	if (m_cur_path_deept >= MAX_PATH_DEEPT) return false;
	
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return false;

	json_object *new_obj = json_object_new_object();
	json_object_array_put_idx(cur_json_obj, index, new_obj);

	m_cur_node_path[m_cur_path_deept++] = new_obj;
	return true;
}

bool JsonParser::ArraySetArrayAndDown(int index)
{
	if (m_cur_path_deept >= MAX_PATH_DEEPT) return false;

	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return false;

	json_object *new_array = json_object_new_array();
	json_object_array_put_idx(cur_json_obj, index, new_array);

	m_cur_node_path[m_cur_path_deept++] = new_array;
	return true;
}

bool JsonParser::ArrayPushBackBoolean(bool value)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return false;

	json_object_array_add(cur_json_obj, json_object_new_boolean(value));
	return true;
}

bool JsonParser::ArrayPushBackInt(int value)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return false;

	json_object_array_add(cur_json_obj, json_object_new_int(value));
	return true;
}

bool JsonParser::ArrayPushBackDouble(double value)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return false;

	json_object_array_add(cur_json_obj, json_object_new_double(value));
	return true;
}

bool JsonParser::ArrayPushBackString(const char *value)
{
	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return false;

	json_object_array_add(cur_json_obj, json_object_new_string(value));
	return true;
}

bool JsonParser::ArrayPushBackObjAndDown()
{
	if (m_cur_path_deept >= MAX_PATH_DEEPT) return false;

	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return false;

	json_object *new_obj = json_object_new_object();
	json_object_array_add(cur_json_obj, new_obj);

	m_cur_node_path[m_cur_path_deept++] = new_obj;
	return true;
}

bool JsonParser::ArrayPushBackArrayAndDown()
{
	if (m_cur_path_deept >= MAX_PATH_DEEPT) return false;

	json_object *cur_json_obj = m_cur_node_path[m_cur_path_deept - 1];
	if (json_object_get_type(cur_json_obj) != json_type_array) return false;

	json_object *new_array = json_object_new_array();
	json_object_array_add(cur_json_obj, new_array);

	m_cur_node_path[m_cur_path_deept++] = new_array;
	return true;
}

const char *JsonParser::ToJsonString()
{
	return json_object_to_json_string(m_json_root);
}

bool JsonParser::SaveInFile( const char *filename )
{
	if(!filename || !m_json_root)
		return false;
	char file_temp[256] = {0};
	ZTStrCpy(file_temp, filename, sizeof(file_temp));
	return json_object_to_file(file_temp, m_json_root) == 0 ? true : false;
}
