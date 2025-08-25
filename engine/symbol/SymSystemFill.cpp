#include "SymSystemFill.h"
#include "JsonUtils.h"
#include "Serialize.h"

SymSystemFill::SymSystemFill() {
    _type = SYM_SYSTEM_FILL;
}

SymSystemFill::~SymSystemFill() {

}


bool SymSystemFill::fromJson(json_object* jsonobj) {
    return SymShape::fromJson(jsonobj);
}


json_object* SymSystemFill::toJson() const {
    json_object* jsonObj = SymShape::toJson();
    // json_object_object_add(jsonObj, "type", json_object_new_string("systemfill"));
    // json_object_object_add(jsonObj, "fill", _fill->toJson());
    return jsonObj;
}



SymShape* SymSystemFill::clone() const {
    SymSystemFill* f = new SymSystemFill();
    f->_type = _type;
    f->_fill = _fill->clone();
    return f;
}


size_t SymSystemFill::memsize() const {
    size_t size = SymShape::memsize();
    return size;
}


char* SymSystemFill::serialize(char* p) const {
    p = SymShape::serialize(p);
    return p;
}

char* SymSystemFill::deserialize(char* p) {
    p = SymShape::deserialize(p);
    return p;
}
