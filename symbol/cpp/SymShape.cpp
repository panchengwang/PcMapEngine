#include "SymShape.h"
#include "JsonUtils.h"


SymShape::SymShape() {
    _type = SYM_NONE;
}


SymShape::~SymShape() {

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
    return _stroke.fromJson(strokeJsonObj, _errorMessage);
}