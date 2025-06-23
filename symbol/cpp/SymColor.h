#ifndef __SYM_COLOR_H__
#define __SYM_COLOR_H__
#include <stdint.h>
#include <json-c/json.h>
#include <string> 

class SymColor
{
public:
    SymColor();
    ~SymColor();

    void setColor(int r, int g, int b, int a = 255);

    bool fromJson(json_object* jsonObj, std::string& errMsg);
    json_object* toJson();

protected:

    uint8_t _red, _green, _blue, _alpha;
};

#endif /* __SYM_COLOR_H__ */
