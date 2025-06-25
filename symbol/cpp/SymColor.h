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
    json_object* toJson() const;

    bool fromHex(const char* hex, std::string& errMsg);

    static uint8_t parse_hex_char(char c);
    static uint8_t parse_hex_byte(const char* hex);
protected:

    uint8_t _red, _green, _blue, _alpha;
};

#endif /* __SYM_COLOR_H__ */
