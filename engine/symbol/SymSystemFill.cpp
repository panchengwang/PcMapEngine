#include "SymSystemFill.h"
#include "JsonUtils.h"
#include "Serialize.h"

SymSystemFill::SymSystemFill() {
    _type = SYM_SYSTEM_FILL;
}

SymSystemFill::~SymSystemFill() {

}


bool SymSystemFill::fromJson(json_object* jsonobj) {
    return SymShape::fillFromJson(jsonobj);
}


json_object* SymSystemFill::toJson() const {
    json_object* jsonObj = SymShape::toJson();
    json_object_object_add(jsonObj, "type", json_object_new_string("systemfill"));
    json_object_object_add(jsonObj, "fill", _fill->toJson());
    return jsonObj;
}



SymShape* SymSystemFill::clone() const {
    SymSystemFill* sym = new SymSystemFill();
    sym->_fill = _fill->clone();
    sym->_type = _type;
    return sym;
}


size_t SymSystemFill::memsize() const {
    return sizeof(_type) + _fill->memsize();
}


char* SymSystemFill::serialize(char* p) const {
    SERIALIZE(p, _type);
    p = _fill->serialize(p);
    return p;
}

char* SymSystemFill::deserialize(char* p) {
    DESERIALIZE(p, _type);
    p = fillDeserialize(p);
    return p;
}
