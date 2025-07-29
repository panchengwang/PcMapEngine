#include "SymArc.h"
#include "../JsonUtils.h"
#include "../Serialize.h"

SymArc::SymArc() {
    _type = SYM_ARC;
}


SymArc::~SymArc() {

}


bool SymArc::fromJson(json_object* jsonobj) {
    if (!SymShape::strokeFromJson(jsonobj)) {
        return false;
    }
    json_object* centerobj;
    JSON_GET_ARRAY_OBJECT(jsonobj, "center", centerobj, _errorMessage);
    if (!_center.fromJson(centerobj, _errorMessage)) {
        return false;
    }
    JSON_GET_DOUBLE(jsonobj, "radiusx", _radiusX, _errorMessage);
    JSON_GET_DOUBLE(jsonobj, "radiusy", _radiusY, _errorMessage);
    JSON_GET_DOUBLE(jsonobj, "rotation", _rotation, _errorMessage);
    JSON_GET_DOUBLE(jsonobj, "startangle", _startAngle, _errorMessage);
    JSON_GET_DOUBLE(jsonobj, "endangle", _endAngle, _errorMessage);
    return true;
}


json_object* SymArc::toJson() const {
    json_object* jsonObj = SymShape::toJson();
    json_object_object_add(jsonObj, "type", json_object_new_string("arc"));
    json_object_object_add(jsonObj, "stroke", _stroke->toJson());
    json_object_object_add(jsonObj, "center", _center.toJson());
    json_object_object_add(jsonObj, "radiusx", json_object_new_double(_radiusX));
    json_object_object_add(jsonObj, "radiusy", json_object_new_double(_radiusY));
    json_object_object_add(jsonObj, "rotation", json_object_new_double(_rotation));
    json_object_object_add(jsonObj, "startangle", json_object_new_double(_startAngle));
    json_object_object_add(jsonObj, "endangle", json_object_new_double(_endAngle));
    return jsonObj;
}



SymShape* SymArc::clone() const {
    SymArc* arc = new SymArc();
    arc->_stroke = _stroke->clone();
    arc->_center = _center;
    arc->_radiusX = _radiusX;
    arc->_radiusY = _radiusY;
    arc->_rotation = _rotation;
    arc->_startAngle = _startAngle;
    arc->_endAngle = _endAngle;
    return arc;
}


double SymArc::radiusX() const {
    return _radiusX;
}


double SymArc::radiusY() const {
    return _radiusY;
}


SymPoint SymArc::center() const {
    return _center;
}


double SymArc::rotation() const {
    return _rotation;
}

double SymArc::startAngle() const {
    return _startAngle;
}

double SymArc::endAngle() const {
    return _endAngle;
}

size_t SymArc::memsize() const {
    size_t size = 0;
    size += sizeof(_type);
    size += _stroke->memsize();
    size += _center.memsize();
    size += sizeof(_radiusX);
    size += sizeof(_radiusY);
    size += sizeof(_rotation);
    size += sizeof(_startAngle);
    size += sizeof(_endAngle);
    return size;
}

char* SymArc::serialize(char* p) const {
    SERIALIZE(p, _type);
    p = _stroke->serialize(p);
    p = _center.serialize(p);
    SERIALIZE(p, _radiusX);
    SERIALIZE(p, _radiusY);
    SERIALIZE(p, _rotation);
    SERIALIZE(p, _startAngle);
    SERIALIZE(p, _endAngle);
    return p;
}

char* SymArc::deserialize(char* data) {
    char* p = data;
    DESERIALIZE(p, _type);
    p = _stroke->deserialize(p);
    p = _center.deserialize(p);
    DESERIALIZE(p, _radiusX);
    DESERIALIZE(p, _radiusY);
    DESERIALIZE(p, _rotation);
    DESERIALIZE(p, _startAngle);
    DESERIALIZE(p, _endAngle);
    return p;
}