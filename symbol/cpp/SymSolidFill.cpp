#include "SymSolidFill.h"
#include "JsonUtils.h"

SymSolidFill::SymSolidFill() {
    _type = SymFill::FILL_SOLID;
}


bool SymSolidFill::fromJson(json_object* json, std::string& errMsg) {
    json_object* objcolor;
    JSON_GET_ARRAY_OBJECT_STRING(json, "color", objcolor, errMsg);
    if (!_color.fromJson(json, errMsg)) {
        return false;
    }

    return true;
}


json_object* SymSolidFill::toJson() const {
    json_object* json = SymFill::toJson();
    json_object_object_add(json, "color", _color.toJson());
    return json;
}

