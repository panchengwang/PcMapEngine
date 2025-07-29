#include "SymSolidFill.h"
#include "JsonUtils.h"
#include "Serialize.h"
#include <iostream>


SymSolidFill::SymSolidFill() {
    _type = SymFill::FILL_SOLID;
}


bool SymSolidFill::fromJson(json_object* json, std::string& errMsg) {
    json_object* objcolor;
    JSON_GET_ARRAY_OBJECT_STRING(json, "color", objcolor, errMsg);
    if (!_color.fromJson(objcolor, errMsg)) {
        return false;
    }
    // std::cerr << "red:" << _color.red()
    //     << " green:" << _color.green()
    //     << " blue:" << _color.blue()
    //     << " alpha:" << _color.alpha()
    //     << std::endl;
    return true;
}


json_object* SymSolidFill::toJson() const {
    json_object* json = SymFill::toJson();
    json_object_object_add(json, "color", _color.toJson());
    return json;
}

SymFill* SymSolidFill::clone() const
{
    SymSolidFill* myfill = new SymSolidFill();
    myfill->_type = _type;
    myfill->_color = _color;
    return myfill;
}



// SkPaint SymSolidFill::toPaint(double dotsPerMM) const {
//     SkPaint paint;
//     paint.setAntiAlias(true);
//     paint.setStyle(SkPaint::kFill_Style);
//     paint.setColor(_color.toSkColor());
//     return paint;
// }

size_t SymSolidFill::memsize() const {
    size_t size = 0;
    size += sizeof(_type);
    size += _color.memsize();
    return size;
}


char* SymSolidFill::serialize(char* p) const {
    SERIALIZE(p, _type);
    p = _color.serialize(p);
    return p;
}

char* SymSolidFill::deserialize(char* data) {
    char* p = data;
    DESERIALIZE(p, _type);
    p = _color.deserialize(p);
    return p;
}