#include "SymShape.h" 
#include "JsonUtils.h"
#include "SymSolidFill.h"
#include <iostream>

SymShape::SymShape() {
    _type = SYM_NONE;
    _stroke = new SymStroke();
    _fill = NULL;
}


SymShape::~SymShape() {
    delete _stroke;
    delete _fill;
}


int SymShape::type() const {
    return _type;
}



bool SymShape::fromJson(json_object* json) {
    return true;
}


json_object* SymShape::toJson() const {
    json_object* jsonObj = json_object_new_object();
    return jsonObj;
}



const std::string& SymShape::getErrorMessage() const {
    return _errorMessage;
}


bool SymShape::strokeFromJson(json_object* jsonobj) {
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

    return _fill->fromJson(fillJsonObj, _errorMessage);
}
