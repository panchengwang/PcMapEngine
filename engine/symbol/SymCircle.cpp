#include "SymCircle.h"
#include "JsonUtils.h"
#include "Serialize.h"
#include "SymSolidFill.h"

SymCircle::SymCircle() {
    _type = SYM_CIRCLE;
}

SymCircle::~SymCircle() {
}


bool SymCircle::fromJson(json_object* jsonObj) {

    if (!SymShape::strokeFromJson(jsonObj)) {
        return false;
    }
    if (!SymShape::fillFromJson(jsonObj)) {
        return false;
    }

    json_object* centerObj;
    if (!json_object_object_get_ex(jsonObj, "center", &centerObj)) {
        _errorMessage = "Missing center in SymCircle";
        return false;
    }

    if (!_center.fromJson(centerObj, _errorMessage)) {
        return false;
    }
    JSON_GET_DOUBLE(jsonObj, "radius", _radius, _errorMessage);

    return true;
}


json_object* SymCircle::toJson() const {
    json_object* jsonObj = SymShape::toJson();
    json_object_object_add(jsonObj, "type", json_object_new_string("circle"));
    json_object_object_add(jsonObj, "stroke", _stroke->toJson());
    json_object_object_add(jsonObj, "fill", _fill->toJson());
    json_object_object_add(jsonObj, "center", _center.toJson());
    json_object_object_add(jsonObj, "radius", json_object_new_double(_radius));
    return jsonObj;
}

SymShape* SymCircle::clone() const {
    SymCircle* c = new SymCircle();
    c->_stroke = _stroke->clone();
    c->_fill = _fill->clone();
    c->_center = _center;
    c->_radius = _radius;
    return c;
}


size_t SymCircle::memsize() const {
    size_t size = 0;
    size += sizeof(_type);
    size += _stroke->memsize();
    size += _fill->memsize();
    size += _center.memsize();
    size += sizeof(_radius);
    return size;
}

char* SymCircle::serialize(char* p) const {
    SERIALIZE(p, _type);
    p = _stroke->serialize(p);
    p = _fill->serialize(p);
    p = _center.serialize(p);
    SERIALIZE(p, _radius);
    return p;
}


char* SymCircle::deserialize(char* data) {
    char* p = data;
    DESERIALIZE(p, _type);
    p = _stroke->deserialize(p);
    p = fillDeserialize(p);
    p = _center.deserialize(p);
    DESERIALIZE(p, _radius);
    return p;
}
