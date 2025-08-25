#include "SymEllipse.h"
#include "JsonUtils.h"
#include "Serialize.h"


SymEllipse::SymEllipse() {
    _type = SYM_ELLIPSE;
}


SymEllipse::~SymEllipse() {

}


bool SymEllipse::fromJson(json_object* jsonobj) {
    // if (!SymShape::strokeFromJson(jsonobj)) {
    //     return false;
    // }
    // if (!SymShape::fillFromJson(jsonobj)) {
    //     return false;
    // }
    if (!SymShape::fromJson(jsonobj)) {
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
    return true;
}


json_object* SymEllipse::toJson() const {
    json_object* jsonObj = SymShape::toJson();
    // json_object_object_add(jsonObj, "type", json_object_new_string("ellipse"));
    // json_object_object_add(jsonObj, "stroke", _stroke->toJson());
    // json_object_object_add(jsonObj, "fill", _fill->toJson());
    json_object_object_add(jsonObj, "center", _center.toJson());
    json_object_object_add(jsonObj, "radiusx", json_object_new_double(_radiusX));
    json_object_object_add(jsonObj, "radiusy", json_object_new_double(_radiusY));
    json_object_object_add(jsonObj, "rotation", json_object_new_double(_rotation));
    return jsonObj;
}



SymShape* SymEllipse::clone() const {
    SymEllipse* ellipse = new SymEllipse();
    ellipse->_type = _type;
    ellipse->_offssetAlongLine = _offssetAlongLine;
    ellipse->_stroke = _stroke->clone();
    ellipse->_fill = _fill->clone();
    ellipse->_center = _center;
    ellipse->_radiusX = _radiusX;
    ellipse->_radiusY = _radiusY;
    ellipse->_rotation = _rotation;
    return ellipse;
}


double SymEllipse::radiusX() const {
    return _radiusX;
}


double SymEllipse::radiusY() const {
    return _radiusY;
}


SymPoint SymEllipse::center() const {
    return _center;
}


double SymEllipse::rotation() const {
    return _rotation;
}


size_t SymEllipse::memsize() const {
    // size_t size = 0;
    // size += sizeof(_type);
    // size += _stroke->memsize();
    // size += _fill->memsize();
    size_t size = SymShape::memsize();
    size += _center.memsize();
    size += sizeof(_radiusX);
    size += sizeof(_radiusY);
    size += sizeof(_rotation);
    return size;
}

char* SymEllipse::serialize(char* p) const {
    // SERIALIZE(p, _type);
    // p = _stroke->serialize(p);
    // p = _fill->serialize(p);
    p = SymShape::serialize(p);
    p = _center.serialize(p);
    SERIALIZE(p, _radiusX);
    SERIALIZE(p, _radiusY);
    SERIALIZE(p, _rotation);
    return p;
}


char* SymEllipse::deserialize(char* data) {
    char* p = data;
    // DESERIALIZE(p, _type);
    // p = _stroke->deserialize(p);
    // p = fillDeserialize(p);
    p = SymShape::deserialize(p);
    p = _center.deserialize(p);
    DESERIALIZE(p, _radiusX);
    DESERIALIZE(p, _radiusY);
    DESERIALIZE(p, _rotation);
    return p;
}