#include "SymChord.h"
#include "Serialize.h"

SymChord::SymChord() {
    _type = SYM_CHORD;
}

SymChord::~SymChord() {

}

bool SymChord::fromJson(json_object* jsonobj) {
    if (!SymArc::fromJson(jsonobj)) {
        return false;
    }

    return true;
}

json_object* SymChord::toJson() const {
    json_object* jsonobj = SymArc::toJson();

    return jsonobj;
}

SymShape* SymChord::clone() const {
    SymChord* chord = new SymChord();
    chord->_type = _type;
    chord->_offssetAlongLine = _offssetAlongLine;
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


size_t SymChord::memsize() const {
    size_t size = SymArc::memsize();
    // size += sizeof(_type);
    // size += sizeof(_offssetAlongLine);
    // size += _stroke->memsize();
    // size += _fill->memsize();
    // size += _center.memsize();
    // size += sizeof(_radiusX);
    // size += sizeof(_radiusY);
    // size += sizeof(_rotation);
    // size += sizeof(_startAngle);
    // size += sizeof(_endAngle);
    return size;
}

char* SymChord::serialize(char* p) const {
    p = SymArc::serialize(p);
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
    return p;
}


char* SymChord::deserialize(char* data) {
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
    p = SymArc::deserialize(p);
    return p;
}