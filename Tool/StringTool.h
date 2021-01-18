
#ifndef _STRING_TOOL_H
#define _STRING_TOOL_H

#include <string>
#include <vector>

class StringTool
{
private:
    StringTool() = delete;
    ~StringTool() = delete;
    StringTool(const StringTool& rhs) = delete;
    StringTool& operator=(const StringTool& rhs) = delete;
    
public:
    static void split(const std::string& str, std::vector<std::string>& v, const char* delimiter = "|");
}

#endif
