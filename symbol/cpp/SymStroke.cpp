#include "SymStroke.h"
#include "JsonUtils.h"
#include <algorithm>
#include <include/effects/SkDashPathEffect.h>


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
    JSON_GET_ARRAY_OBJ(jsonObj, "dashes", dashesObj, errMsg);
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



SkPaint SymStroke::toPaint(double dotsPerMM) const {
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStrokeWidth(_width * dotsPerMM);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setColor(_color.toSkColor());
    float intervals[_dashes.size()];
    for (int i = 0; i < _dashes.size(); i++) {
        intervals[i] = _dashes[i] * dotsPerMM;
    }
    paint.setPathEffect(SkDashPathEffect::Make(intervals, _dashes.size(), 0.0f));
    return paint;
}