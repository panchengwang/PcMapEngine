#include "SymPie.h"
#include "JsonUtils.h"
#include "Serialize.h"


SymPie::SymPie() {
    _type = SYM_PIE;
}

SymPie::~SymPie() {

}

bool SymPie::fromJson(json_object* jsonobj) {
    if (!SymChord::fromJson(jsonobj)) {
        return false;
    }
    return true;
}

json_object* SymPie::toJson() const {
    json_object* jsonobj = SymChord::toJson();
    return jsonobj;
}

SymShape* SymPie::clone() const {
    SymPie* pie = new SymPie();
    pie->_type = _type;
    pie->_offssetAlongLine = _offssetAlongLine;
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


size_t SymPie::memsize() const {
    size_t size = SymChord::memsize();
    return size;
}


char* SymPie::serialize(char* p) const {
    // SERIALIZE(p, _type);
    // SERIALIZE(p, _offssetAlongLine);
    // p = _stroke->serialize(p);
    // p = _fill->serialize(p);
    // p = _center.serialize(p);
    // SERIALIZE(p, _radiusX);
    // SERIALIZE(p, _radiusY);
    // SERIALIZE(p, _rotation);
    // SERIALIZE(p, _startAngle);
    // SERIALIZE(p, _endAngle);
    p = SymChord::serialize(p);
    return p;
}

char* SymPie::deserialize(char* data) {
    char* p = data;
    // DESERIALIZE(p, _type);
    // DESERIALIZE(p, _offssetAlongLine);
    // p = _stroke->deserialize(p);
    // p = fillDeserialize(p);
    // p = _center.deserialize(p);
    // DESERIALIZE(p, _radiusX);
    // DESERIALIZE(p, _radiusY);
    // DESERIALIZE(p, _rotation);
    // DESERIALIZE(p, _startAngle);
    // DESERIALIZE(p, _endAngle);
    p = SymChord::deserialize(p);
    return p;
}