#include "SymStroke.h"
#include "JsonUtils.h"
#include "Serialize.h"

#include <algorithm>
// #include <include/effects/SkDashPathEffect.h> 


#define TO_LOWER(str)                                                           \
{                                                                               \
std::transform(str.begin(), str.end(), str.begin(), ::tolower);                 \
}


SymStroke::SymStroke() {

}


SymStroke::~SymStroke() {
    _dashes.clear();
}


bool SymStroke::fromJson(json_object* jsonObj, std::string& errMsg) {
    std::string capstr;

    JSON_GET_DOUBLE(jsonObj, "width", _width, errMsg);

    JSON_GET_STRING(jsonObj, "cap", capstr, errMsg);
    TO_LOWER(capstr);
    if (capstr == "butt") {
        _cap = CAP_BUTT;
    }
    else if (capstr == "round") {
        _cap = CAP_ROUND;
    }
    else if (capstr == "square") {
        _cap = CAP_SQUARE;
    }
    else {
        errMsg = "Invalid stroke cap style: " + capstr;
        return false;
    }

    std::string joinstr;
    JSON_GET_STRING(jsonObj, "join", joinstr, errMsg);
    TO_LOWER(joinstr);
    if (joinstr == "miter") {
        _join = JOIN_MITER;
    }
    else if (joinstr == "round") {
        _join = JOIN_ROUND;
    }
    else if (joinstr == "bevel") {
        _join = JOIN_BEVEL;
    }
    else {
        errMsg = "Invalid stroke join style: " + joinstr;
        return false;
    }

    json_object* colorObj;
    JSON_GET_ARRAY_OBJECT_STRING(jsonObj, "color", colorObj, errMsg);
    if (!_color.fromJson(colorObj, errMsg)) {
        errMsg = "Invalid color: " + errMsg;
        return false;
    }

    _dashes.clear();
    json_object* dashesObj;
    JSON_GET_ARRAY(jsonObj, "dashes", dashesObj, errMsg);
    for (size_t i = 0; i < json_object_array_length(dashesObj); i++)
    {
        json_object* dashObj = json_object_array_get_idx(dashesObj, i);
        double dash = json_object_get_double(dashObj);
        _dashes.push_back(dash);
    }

    return true;
}


json_object* SymStroke::toJson() const {
    json_object* jsonObj = json_object_new_object();
    json_object_object_add(jsonObj, "width", json_object_new_double(_width));

    switch (_cap) {
    case CAP_BUTT:
        json_object_object_add(jsonObj, "cap", json_object_new_string("butt"));
        break;
    case CAP_ROUND:
        json_object_object_add(jsonObj, "cap", json_object_new_string("round"));
        break;
    case CAP_SQUARE:
        json_object_object_add(jsonObj, "cap", json_object_new_string("square"));
        break;
    }

    switch (_join) {
    case JOIN_MITER:
        json_object_object_add(jsonObj, "join", json_object_new_string("miter"));
        break;
    case JOIN_ROUND:
        json_object_object_add(jsonObj, "join", json_object_new_string("round"));
        break;
    case JOIN_BEVEL:
        json_object_object_add(jsonObj, "join", json_object_new_string("bevel"));
        break;
    }
    json_object_object_add(jsonObj, "color", _color.toJson());

    json_object* dashesObj = json_object_new_array();
    for (auto dash : _dashes) {
        json_object_array_add(dashesObj, json_object_new_double(dash));
    }
    json_object_object_add(jsonObj, "dashes", dashesObj);

    return jsonObj;
}


SymStroke::SymStroke(const SymStroke& stroke) {
    _width = stroke._width;
    _cap = stroke._cap;
    _join = stroke._join;
    for (auto dash : stroke._dashes) {
        _dashes.push_back(dash);
    }
    _color = stroke._color;
}

SymStroke* SymStroke::clone() const
{
    return new SymStroke(*this);
}


size_t SymStroke::memsize() const {
    size_t size = 0;
    size += sizeof(_width);
    size += sizeof(_cap);
    size += sizeof(_join);
    size += sizeof(size_t);
    size += _dashes.size() * sizeof(double);
    size += _color.memsize();
    return size;
}


char* SymStroke::serialize(char* p) const {
    SERIALIZE(p, _width);
    SERIALIZE(p, _cap);
    SERIALIZE(p, _join);
    int32_t ndashes = _dashes.size();
    SERIALIZE(p, ndashes);
    for (auto dash : _dashes) {
        SERIALIZE(p, dash);
    }
    p = _color.serialize(p);
    return p;
}


char* SymStroke::deserialize(char* data) {
    char* p = data;
    DESERIALIZE(p, _width);
    DESERIALIZE(p, _cap);
    DESERIALIZE(p, _join);
    int32_t ndashes;
    DESERIALIZE(p, ndashes);
    _dashes.clear();
    for (int i = 0; i < ndashes; i++) {
        double dash;
        DESERIALIZE(p, dash);
        _dashes.push_back(dash);
    }
    p = _color.deserialize(p);
    return p;
}


const std::vector<double>& SymStroke::dashes() const {
    return _dashes;
}

double SymStroke::width() const {
    return _width;
}

SymStroke::CapStyle SymStroke::cap() const {
    return _cap;
}

SymStroke::JoinStyle SymStroke::join() const {
    return _join;
}

const SymColor& SymStroke::color() const {
    return _color;
}
