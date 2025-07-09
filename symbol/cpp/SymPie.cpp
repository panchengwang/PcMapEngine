#include "SymPie.h"

SymPie::SymPie() {
    _type = SYM_PIE;
}

SymPie::~SymPie() {

}

bool SymPie::fromJson(json_object* jsonobj) {
    if (!SymChord::fromJson(jsonobj)) {
        return false;
    }
    _type = SYM_PIE;
    return true;
}

json_object* SymPie::toJson() const {
    json_object* jsonobj = SymChord::toJson();

    json_object_object_add(jsonobj, "type", json_object_new_string("pie"));

    return jsonobj;
}

SymShape* SymPie::clone() const {
    SymPie* pie = new SymPie();
    pie->_type = _type;
    pie->_stroke = _stroke->clone();
    pie->_fill = _fill->clone();
    pie->_center = _center;
    pie->_radiusX = _radiusX;
    pie->_radiusY = _radiusY;
    pie->_startAngle = _startAngle;
    pie->_endAngle = _endAngle;
    pie->_rotation = _rotation;
    return pie;
}
