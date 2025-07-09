#include "SymPolygon.h"
#include "JsonUtils.h"


SymPolygon::SymPolygon()
{
    _type = SYM_POLYGON;
}

SymPolygon::~SymPolygon()
{
}


bool SymPolygon::fromJson(json_object* jsonobj) {
    if (!SymShape::strokeFromJson(jsonobj)) {
        return false;
    }
    if (!SymShape::fillFromJson(jsonobj)) {
        return false;
    }

    json_object* pointarr;
    JSON_GET_ARRAY(jsonobj, "points", pointarr, _errorMessage);
    int len = json_object_array_length(pointarr);
    _points.clear();
    for (int i = 0; i < len; ++i) {
        json_object* ptobj;
        ptobj = json_object_array_get_idx(pointarr, i);
        SymPoint pt;
        if (!pt.fromJson(ptobj, _errorMessage)) {
            return false;
        }
        _points.push_back(pt);
    }
    return true;
}


json_object* SymPolygon::toJson() const {
    json_object* jsonObj = SymShape::toJson();
    json_object_object_add(jsonObj, "type", json_object_new_string("polygon"));
    json_object_object_add(jsonObj, "stroke", _stroke->toJson());
    json_object_object_add(jsonObj, "fill", _fill->toJson());

    json_object* pointarr = json_object_new_array();
    for (auto& pt : _points) {
        json_object* ptobj = pt.toJson();
        json_object_array_add(pointarr, ptobj);
    }
    json_object_object_add(jsonObj, "points", pointarr);

    return jsonObj;
}



SymShape* SymPolygon::clone() const {
    SymPolygon* newPolygon = new SymPolygon();
    newPolygon->_stroke = _stroke->clone();
    newPolygon->_fill = _fill->clone();
    for (auto& pt : _points) {
        newPolygon->_points.push_back(pt);
    }
    return newPolygon;
}


const std::vector<SymPoint>& SymPolygon::points() const {
    return _points;
}

