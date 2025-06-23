#include "SymColor.h"
#include "JsonUtils.h"

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


bool SymColor::fromJson(json_object* jsonObj, std::string& errMsg) {
    JSON_GET_INT(jsonObj, "red", _red, errMsg);
    JSON_GET_INT(jsonObj, "green", _green, errMsg);
    JSON_GET_INT(jsonObj, "blue", _blue, errMsg);
    JSON_GET_INT(jsonObj, "alpha", _alpha, errMsg);
    return true;
}



json_object* SymColor::toJson() {
    json_object* jsonObj = json_object_new_object();
    json_object_object_add(jsonObj, "red", json_object_new_int(_red));
    json_object_object_add(jsonObj, "green", json_object_new_int(_green));
    json_object_object_add(jsonObj, "blue", json_object_new_int(_blue));
    json_object_object_add(jsonObj, "alpha", json_object_new_int(_alpha));
    return jsonObj;
}


