#include "SymColor.h"
#include "JsonUtils.h"
#include "Serialize.h"
#include <string.h>

SymColor::SymColor() {
    _red = 0;
    _green = 0;
    _blue = 0;
    _alpha = 255;
}


SymColor::~SymColor() {

}



void SymColor::setColor(int r, int g, int b, int a) {
    _red = r;
    _green = g;
    _blue = b;
    _alpha = a;
}


uint8_t SymColor::parse_hex_char(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

uint8_t SymColor::parse_hex_byte(const char* hex) {
    return (SymColor::parse_hex_char(hex[0]) << 4) | SymColor::parse_hex_char(hex[1]);
}


bool SymColor::fromHex(const char* hex, std::string& errMsg) {
    int len = strlen(hex);
    if (len == 7 || len == 9) { // #RRGGBB 或 #RRGGBBAA
        _red = parse_hex_byte(hex + 1);
        _green = parse_hex_byte(hex + 3);
        _blue = parse_hex_byte(hex + 5);
        if (len == 9)
            _alpha = parse_hex_byte(hex + 7);
    }
    else {
        errMsg = "color: invalid color string";
        return false;
    }
    return true;
}

bool SymColor::fromJson(json_object* jsonObj, std::string& errMsg) {
    json_type type = json_object_get_type(jsonObj);
    if (type == json_type_array) {
        int len = json_object_array_length(jsonObj);
        if (len != 4 && len != 3) {
            errMsg = "color: array length is not equal to 3 or 4";
            return false;
        }

        for (int i = 0; i < len; ++i) {
            json_object* item = json_object_array_get_idx(jsonObj, i);
            if (i < 3) {
                int val = json_object_get_int(item);
                switch (i) {
                case 0:
                    _red = val;
                    break;
                case 1:
                    _green = val;
                    break;
                case 2:
                    _blue = val;
                    break;
                }
            }
            else {
                _alpha = json_object_get_int(item);
            }
        }
    }
    else if (type == json_type_object) {
        JSON_GET_INT(jsonObj, "red", _red, errMsg);
        JSON_GET_INT(jsonObj, "green", _green, errMsg);
        JSON_GET_INT(jsonObj, "blue", _blue, errMsg);
        JSON_GET_INT(jsonObj, "alpha", _alpha, errMsg);
    }
    else if (type == json_type_string) {
        std::string str = json_object_get_string(jsonObj);
        if (!fromHex(str.c_str(), errMsg)) {
            return false;
        }
    }
    else {
        errMsg = "color: invalid color type";
        return false;
    }

    return true;
}



json_object* SymColor::toJson() const {
    json_object* jsonObj = json_object_new_array();
    json_object_array_add(jsonObj, json_object_new_int(_red));
    json_object_array_add(jsonObj, json_object_new_int(_green));
    json_object_array_add(jsonObj, json_object_new_int(_blue));
    json_object_array_add(jsonObj, json_object_new_int(_alpha));
    return jsonObj;
}


size_t SymColor::memsize() const {
    return 4;
}


char* SymColor::serialize(char* p) const {
    SERIALIZE(p, _red);
    SERIALIZE(p, _green);
    SERIALIZE(p, _blue);
    SERIALIZE(p, _alpha);
    return p;
}


char* SymColor::deserialize(char* data) {
    char* p = data;
    DESERIALIZE(p, _red);
    DESERIALIZE(p, _green);
    DESERIALIZE(p, _blue);
    DESERIALIZE(p, _alpha);
    return p;
}