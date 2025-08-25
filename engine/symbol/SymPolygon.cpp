#include "SymPolygon.h"
#include "JsonUtils.h"
#include "Serialize.h"



SymPolygon::SymPolygon()
{
    _type = SYM_POLYGON;
}

SymPolygon::~SymPolygon()
{
}


bool SymPolygon::fromJson(json_object* jsonobj) {
    if (!SymLineString::fromJson(jsonobj)) {
        return false;
    }

    // json_object* pointarr;
    // JSON_GET_ARRAY(jsonobj, "points", pointarr, _errorMessage);
    // size_t len = json_object_array_length(pointarr);
    // _points.clear();
    // for (size_t i = 0; i < len; ++i) {
    //     json_object* ptobj;
    //     ptobj = json_object_array_get_idx(pointarr, i);
    //     SymPoint pt;
    //     if (!pt.fromJson(ptobj, _errorMessage)) {
    //         return false;
    //     }
    //     _points.push_back(pt);
    // }
    return true;
}


json_object* SymPolygon::toJson() const {
    json_object* jsonObj = SymLineString::toJson();
    // json_object_object_add(jsonObj, "type", json_object_new_string("polygon"));
    // json_object_object_add(jsonObj, "stroke", _stroke->toJson());
    // json_object_object_add(jsonObj, "fill", _fill->toJson());

    // json_object* pointarr = json_object_new_array();
    // for (auto& pt : _points) {
    //     json_object* ptobj = pt.toJson();
    //     json_object_array_add(pointarr, ptobj);
    // }
    // json_object_object_add(jsonObj, "points", pointarr);

    return jsonObj;
}



SymShape* SymPolygon::clone() const {
    SymPolygon* newPolygon = new SymPolygon();
    newPolygon->_type = _type;
    newPolygon->_offssetAlongLine = _offssetAlongLine;
    newPolygon->_stroke = _stroke->clone();
    newPolygon->_fill = _fill->clone();
    for (auto& pt : _points) {
        newPolygon->_points.push_back(pt);
    }
    return newPolygon;
}



size_t SymPolygon::memsize() const {
    size_t size = SymLineString::memsize();
    // size += sizeof(_type);
    // size += _stroke->memsize();
    // size += _fill->memsize();
    // size += sizeof(int32_t);
    // for (auto& pt : _points) {
    //     size += pt.memsize();
    // }
    return size;
}


char* SymPolygon::serialize(char* p) const {
    // SERIALIZE(p, _type);
    // p = _stroke->serialize(p);
    // p = _fill->serialize(p);
    // int32_t npoints = _points.size();
    // SERIALIZE(p, npoints);
    // for (auto& pt : _points) {
    //     p = pt.serialize(p);
    // }
    p = SymLineString::serialize(p);
    return p;
}


char* SymPolygon::deserialize(char* data) {
    char* p = data;
    // DESERIALIZE(p, _type);
    // p = _stroke->deserialize(p);
    // p = fillDeserialize(p);
    // int32_t npoints;
    // DESERIALIZE(p, npoints);
    // for (int i = 0; i < npoints; ++i) {
    //     SymPoint pt;
    //     p = pt.deserialize(p);
    //     _points.push_back(pt);
    // }
    p = SymLineString::deserialize(p);
    return p;
}