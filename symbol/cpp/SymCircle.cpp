#include "SymCircle.h"
#include "JsonUtils.h"


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
