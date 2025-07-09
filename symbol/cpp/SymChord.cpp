#include "SymChord.h"


SymChord::SymChord() {
    _type = SYM_CHORD;
}

SymChord::~SymChord() {

}

bool SymChord::fromJson(json_object* jsonobj) {
    if (!SymArc::fromJson(jsonobj)) {
        return false;
    }
    if (!SymShape::fillFromJson(jsonobj)) {
        return false;
    }
    return true;
}

json_object* SymChord::toJson() const {
    json_object* jsonobj = SymArc::toJson();
    json_object_object_add(jsonobj, "type", json_object_new_string("chord"));
    json_object* fillobj = _fill->toJson();
    json_object_object_add(jsonobj, "fill", fillobj);
    return jsonobj;
}

SymShape* SymChord::clone() const {
    SymChord* chord = new SymChord();
    chord->_center = _center;
    chord->_radiusX = _radiusX;
    chord->_radiusY = _radiusY;
    chord->_startAngle = _startAngle;
    chord->_endAngle = _endAngle;
    chord->_fill = _fill->clone();
    chord->_stroke = _stroke->clone();
    chord->_rotation = _rotation;
    return chord;
}

