#include "SymLineString.h"
#include "../JsonUtils.h"
#include "../Serialize.h"


SymLineString::SymLineString()
{
    _type = SYM_LINESTRING;
}

SymLineString::~SymLineString()
{
}


bool SymLineString::fromJson(json_object* jsonobj) {
    if (!SymShape::strokeFromJson(jsonobj)) {
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


json_object* SymLineString::toJson() const {
    json_object* jsonObj = SymShape::toJson();
    json_object_object_add(jsonObj, "type", json_object_new_string("linestring"));
    json_object_object_add(jsonObj, "stroke", _stroke->toJson());

    json_object* pointarr = json_object_new_array();
    for (auto& pt : _points) {
        json_object* ptobj = pt.toJson();
        json_object_array_add(pointarr, ptobj);
    }
    json_object_object_add(jsonObj, "points", pointarr);

    return jsonObj;
}



SymShape* SymLineString::clone() const {
    SymLineString* newLineString = new SymLineString();
    newLineString->_stroke = _stroke->clone();
    for (auto& pt : _points) {
        newLineString->_points.push_back(pt);
    }
    return newLineString;
}


const std::vector<SymPoint>& SymLineString::points() const {
    return _points;
}


size_t SymLineString::memsize() const {
    size_t size = 0;
    size += sizeof(_type);
    size += _stroke->memsize();
    size += sizeof(int32_t);
    for (auto& pt : _points) {
        size += pt.memsize();
    }
    return size;
}


char* SymLineString::serialize(char* p) const {
    SERIALIZE(p, _type);
    p = _stroke->serialize(p);
    int32_t npoints = _points.size();
    SERIALIZE(p, npoints);
    for (auto& pt : _points) {
        p = pt.serialize(p);
    }
    return p;
}

char* SymLineString::deserialize(char* data) {
    char* p = data;
    DESERIALIZE(p, _type);
    p = _stroke->deserialize(p);
    int32_t npoints;
    DESERIALIZE(p, npoints);
    _points.clear();
    for (int i = 0; i < npoints; ++i) {
        SymPoint pt;
        p = pt.deserialize(p);
        _points.push_back(pt);
    }
    return p;
}