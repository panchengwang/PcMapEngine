#include "SymRegularPolygon.h"
#include "JsonUtils.h"
#include "Serialize.h"
#include <cmath>
#include <math.h>

SymRegularPolygon::SymRegularPolygon() {
    _type = SYM_REGULAR_POLYGON;
    _center = SymPoint(0, 0);
    _radius = 0.5;
    _sides = 4;
    _rotation = 0;
}


SymRegularPolygon::~SymRegularPolygon() {

}


bool SymRegularPolygon::fromJson(json_object* jsonobj) {
    if (!SymShape::strokeFromJson(jsonobj)) {
        return false;
    }

    if (!SymShape::fillFromJson(jsonobj)) {
        return false;
    }

    json_object* centerobj;
    JSON_GET_ARRAY_OBJECT(jsonobj, "center", centerobj, _errorMessage);
    if (!_center.fromJson(centerobj, _errorMessage)) {
        return false;
    }
    JSON_GET_DOUBLE(jsonobj, "radius", _radius, _errorMessage);
    JSON_GET_INT(jsonobj, "sides", _sides, _errorMessage);
    JSON_GET_DOUBLE(jsonobj, "rotation", _rotation, _errorMessage);
    return true;
}


json_object* SymRegularPolygon::toJson() const {
    json_object* jsonObj = SymShape::toJson();
    json_object_object_add(jsonObj, "type", json_object_new_string("regularpolygon"));
    json_object_object_add(jsonObj, "stroke", _stroke->toJson());
    json_object_object_add(jsonObj, "fill", _fill->toJson());
    json_object_object_add(jsonObj, "center", _center.toJson());
    json_object_object_add(jsonObj, "radius", json_object_new_double(_radius));
    json_object_object_add(jsonObj, "sides", json_object_new_int(_sides));
    json_object_object_add(jsonObj, "rotation", json_object_new_double(_rotation));
    return jsonObj;
}



SymShape* SymRegularPolygon::clone() const {
    SymRegularPolygon* rpg = new SymRegularPolygon();
    rpg->_type = _type;
    rpg->_stroke = _stroke->clone();
    rpg->_fill = _fill->clone();
    rpg->_center = _center;
    rpg->_radius = _radius;
    rpg->_sides = _sides;
    rpg->_rotation = _rotation;
    return rpg;
}



std::vector<SymPoint> SymRegularPolygon::getPoints() const {
    std::vector<SymPoint> points;
    double step = 2 * M_PI / _sides;
    double angle = _sides % 2 == 0 ? step * 0.5 : 0;
    angle += M_PI * 0.5;
    for (size_t i = 0; i < _sides; ++i) {
        double x = _radius * cos(angle);
        double y = _radius * sin(angle);
        points.push_back(SymPoint(x, y));
        angle += step;
    }
    return points;
}


size_t SymRegularPolygon::memsize() const {
    size_t size = 0;
    size += sizeof(_type);
    size += _stroke->memsize();
    size += _fill->memsize();
    size += _center.memsize();
    size += sizeof(_radius);
    size += sizeof(_sides);
    size += sizeof(_rotation);
    return size;
}


char* SymRegularPolygon::serialize(char* p) const {
    SERIALIZE(p, _type);
    p = _stroke->serialize(p);
    p = _fill->serialize(p);
    p = _center.serialize(p);
    SERIALIZE(p, _radius);
    SERIALIZE(p, _sides);
    SERIALIZE(p, _rotation);
    return p;
}


char* SymRegularPolygon::deserialize(char* data) {
    char* p = data;
    DESERIALIZE(p, _type);
    p = _stroke->deserialize(p);
    p = fillDeserialize(p);
    p = _center.deserialize(p);
    DESERIALIZE(p, _radius);
    DESERIALIZE(p, _sides);
    DESERIALIZE(p, _rotation);
    return p;
}