#include "SymShape.h" 
#include "JsonUtils.h"
#include "Serialize.h"

#include "SymSolidFill.h"

#include <iostream>

SymShape::SymShape() {
    _type = SYM_NONE;
    // _stroke = new SymStroke();
    _stroke = NULL;
    _fill = NULL;
}


SymShape::~SymShape() {
    delete _stroke;
    delete _fill;
}


uint8_t SymShape::type() const {
    return _type;
}



bool SymShape::fromJson(json_object* json) {
    if (!typeFromJson(json)) {
        return false;
    }
    JSON_GET_DOUBLE_WITH_DEFAULT(json, "offsetalongline", _offssetAlongLine, _errorMessage, 0);
    if (_type != SYM_SYSTEM_FILL) {
        if (!strokeFromJson(json)) {
            return false;
        }
    }

    if (_type != SYM_SYSTEM_LINE && _type != SYM_LINESTRING && _type != SYM_ARC) {
        if (!fillFromJson(json)) {
            return false;
        }
    }



    return true;
}


bool SymShape::typeFromJson(json_object* jsonobj) {
    std::string typestr;
    JSON_GET_STRING(jsonobj, "type", typestr, _errorMessage);

    if (typestr == "systemline") {
        _type = SYM_SYSTEM_LINE;
    }
    else if (typestr == "systemfill") {
        _type = SYM_SYSTEM_FILL;
    }
    else if (typestr == "arc") {
        _type = SYM_ARC;
    }
    else if (typestr == "circle") {
        _type = SYM_CIRCLE;
    }
    else if (typestr == "ellipse") {
        _type = SYM_ELLIPSE;
    }
    else if (typestr == "linestring") {
        _type = SYM_LINESTRING;
    }
    else if (typestr == "pie") {
        _type = SYM_PIE;
    }
    else if (typestr == "chord") {
        _type = SYM_CHORD;
    }
    else if (typestr == "polygon") {
        _type = SYM_POLYGON;
    }
    else if (typestr == "regularpolygon") {
        _type = SYM_REGULAR_POLYGON;
    }
    else if (typestr == "rectangle") {
        _type = SYM_RECTANGLE;
    }
    else if (typestr == "star") {
        _type = SYM_STAR;
    }
    else if (typestr == "text") {
        _type = SYM_TEXT;
    }
    else {
        _errorMessage = "Invalid shape type: " + typestr;
        return false;
    }
    return true;
}

json_object* SymShape::toJson() const {
    json_object* jsonObj = json_object_new_object();

    if (_type == SYM_SYSTEM_LINE) {
        json_object_object_add(jsonObj, "type", json_object_new_string("systemline"));
    }
    else if (_type == SYM_SYSTEM_FILL) {
        json_object_object_add(jsonObj, "type", json_object_new_string("systemfill"));
    }
    else if (_type == SYM_ARC) {
        json_object_object_add(jsonObj, "type", json_object_new_string("arc"));
    }
    else if (_type == SYM_CIRCLE) {
        json_object_object_add(jsonObj, "type", json_object_new_string("circle"));
    }
    else if (_type == SYM_ELLIPSE) {
        json_object_object_add(jsonObj, "type", json_object_new_string("ellipse"));
    }
    else if (_type == SYM_LINESTRING) {
        json_object_object_add(jsonObj, "type", json_object_new_string("linestring"));
    }
    else if (_type == SYM_PIE) {
        json_object_object_add(jsonObj, "type", json_object_new_string("pie"));
    }
    else if (_type == SYM_CHORD) {
        json_object_object_add(jsonObj, "type", json_object_new_string("chord"));
    }
    else if (_type == SYM_POLYGON) {
        json_object_object_add(jsonObj, "type", json_object_new_string("polygon"));
    }
    else if (_type == SYM_REGULAR_POLYGON) {
        json_object_object_add(jsonObj, "type", json_object_new_string("regularpolygon"));
    }
    else if (_type == SYM_RECTANGLE) {
        json_object_object_add(jsonObj, "type", json_object_new_string("rectangle"));
    }
    else if (_type == SYM_STAR) {
        json_object_object_add(jsonObj, "type", json_object_new_string("star"));
    }
    else if (_type == SYM_TEXT) {
        json_object_object_add(jsonObj, "type", json_object_new_string("text"));
    }


    if (_type == SYM_SYSTEM_LINE || _type == SYM_SYSTEM_FILL) {
        json_object_object_add(jsonObj, "offsetalongline", json_object_new_double(_offssetAlongLine));
    }
    if (_stroke) {
        json_object_object_add(jsonObj, "stroke", _stroke->toJson());
    }
    if (_fill) {
        json_object_object_add(jsonObj, "fill", _fill->toJson());
    }



    return jsonObj;
}



const std::string& SymShape::getErrorMessage() const {
    return _errorMessage;
}


bool SymShape::strokeFromJson(json_object* jsonobj) {
    if (_stroke) {
        delete _stroke;
        _stroke = NULL;
    }
    _stroke = new SymStroke();
    json_object* strokeJsonObj;
    JSON_GET_OBJECT(jsonobj, "stroke", strokeJsonObj, _errorMessage);
    return _stroke->fromJson(strokeJsonObj, _errorMessage);
}


bool SymShape::fillFromJson(json_object* jsonobj) {
    json_object* fillJsonObj;
    JSON_GET_OBJECT(jsonobj, "fill", fillJsonObj, _errorMessage);

    if (_fill) {
        delete _fill;
        _fill = NULL;
    }
    std::string typestr;
    JSON_GET_STRING(fillJsonObj, "type", typestr, _errorMessage);

    if (typestr == "solid") {
        _fill = new SymSolidFill();
    }
    else {
        _errorMessage = "Invalid fill type: " + typestr;
        return false;
    }

    if (!_fill) {
        _errorMessage = "Failed to create fill of type: " + typestr;
        return false;
    }
    return _fill->fromJson(fillJsonObj, _errorMessage);
}


char* SymShape::fillDeserialize(char* data) {
    char* p = data;
    uint8_t type;
    memcpy(&type, p, sizeof(uint8_t));
    if (_fill) {
        delete _fill;
    }
    if (type == SymFill::FILL_SOLID) {
        _fill = new SymSolidFill();
    }
    p = _fill->deserialize(p);
    return p;
}

char* SymShape::strokeDeserialize(char* data) {
    char* p = data;
    if (_stroke) {
        delete _stroke;
    }
    _stroke = new SymStroke();
    p = _stroke->deserialize(p);
    return p;
}

size_t SymShape::memsize() const {
    size_t size = 0;
    size += sizeof(_type);
    size += sizeof(double); // _offsetAlongLine
    if (_stroke) {
        size += _stroke->memsize();
    }
    if (_fill) {
        size += _fill->memsize();
    }
    return size;
}


char* SymShape::serialize(char* buf) const {
    char* p = buf;
    SERIALIZE(p, _type);
    SERIALIZE(p, _offssetAlongLine);
    if (_stroke) {
        p = _stroke->serialize(p);
    }
    if (_fill) {
        p = _fill->serialize(p);
    }
    return p;
}


char* SymShape::deserialize(char* data) {
    char* p = data;
    DESERIALIZE(p, _type);
    DESERIALIZE(p, _offssetAlongLine);
    if (_type != SYM_SYSTEM_FILL) {
        p = strokeDeserialize(p);
    }
    if (_type != SYM_SYSTEM_LINE && _type != SYM_LINESTRING && _type != SYM_ARC) {
        p = fillDeserialize(p);
    }
    return p;
}