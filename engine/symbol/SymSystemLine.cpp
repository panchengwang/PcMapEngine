#include "SymSystemLine.h"
#include "JsonUtils.h"
#include "Serialize.h"
#include <iostream>

SymSystemLine::SymSystemLine() {
    _type = SYM_SYSTEM_LINE;
}

SymSystemLine::~SymSystemLine() {

}


bool SymSystemLine::fromJson(json_object* jsonobj) {
    return SymShape::fromJson(jsonobj);
}


json_object* SymSystemLine::toJson() const {
    json_object* jsonObj = SymShape::toJson();
    // json_object_object_add(jsonObj, "type", json_object_new_string("systemline"));
    // json_object_object_add(jsonObj, "stroke", _stroke->toJson());
    return jsonObj;
}



SymShape* SymSystemLine::clone() const {
    SymSystemLine* sym = new SymSystemLine();
    sym->_type = _type;
    sym->_stroke = _stroke->clone();
    return sym;
}


size_t SymSystemLine::memsize() const {
    size_t size = SymShape::memsize();
    return size;
}


char* SymSystemLine::serialize(char* p) const {
    // SERIALIZE(p, _type);
    // p = _stroke->serialize(p);
    p = SymShape::serialize(p);
    return p;
}

char* SymSystemLine::deserialize(char* p) {
    // DESERIALIZE(p, _type);
    // p = _stroke->deserialize(p);
    p = SymShape::deserialize(p);
    return p;
}
