#ifndef __SYM_COLOR_H__
#define __SYM_COLOR_H__
#include <stdint.h>
#include <json-c/json.h>
#include <string> 
// #include <SkColor.h>

class SymCanvas;
class SymColor
{
    friend class SymCanvas;
public:
    SymColor();
    ~SymColor();

    void setColor(int r, int g, int b, int a = 255);

    bool fromJson(json_object* jsonObj, std::string& errMsg);
    json_object* toJson() const;

    bool fromHex(const char* hex, std::string& errMsg);

    static uint8_t parse_hex_char(char c);
    static uint8_t parse_hex_byte(const char* hex);

    size_t memsize() const;
    char* serialize(char* p) const;
    char* deserialize(char* data);
    // SkColor toSkColor() const {
    //     return SkColorSetARGB(_alpha, _red, _green, _blue);
    // }

    uint8_t red() const {
        return _red;
    }

    uint8_t green() const {
        return _green;
    }

    uint8_t blue() const {
        return _blue;
    }

    uint8_t alpha() const {
        return _alpha;
    }

protected:

    uint8_t _red, _green, _blue, _alpha;
};

#endif /* __SYM_COLOR_H__ */
