#include "SymRectangle.h"
#include "JsonUtils.h"

SymRectangle::SymRectangle() {
    _type = SYM_RECTANGLE;
    _minX = _minY = _maxX = _maxY = 0;
}



SymRectangle::~SymRectangle() {

}



bool SymRectangle::fromJson(json_object* jsonobj) {
    if (!SymShape::strokeFromJson(jsonobj)) {
        return false;
    }
    if (!SymShape::fillFromJson(jsonobj)) {
        return false;
    }

    JSON_GET_DOUBLE(jsonobj, "minx", _minX, _errorMessage);
    JSON_GET_DOUBLE(jsonobj, "miny", _minY, _errorMessage);
    JSON_GET_DOUBLE(jsonobj, "maxx", _maxX, _errorMessage);
    JSON_GET_DOUBLE(jsonobj, "maxy", _maxY, _errorMessage);
    return true;
}



json_object* SymRectangle::toJson() const {
    json_object* jsonObj = SymShape::toJson();
    json_object_object_add(jsonObj, "type", json_object_new_string("rectangle"));
    json_object_object_add(jsonObj, "stroke", _stroke->toJson());
    json_object_object_add(jsonObj, "fill", _fill->toJson());
    json_object_object_add(jsonObj, "minx", json_object_new_double(_minX));
    json_object_object_add(jsonObj, "miny", json_object_new_double(_minY));
    json_object_object_add(jsonObj, "maxx", json_object_new_double(_maxX));
    json_object_object_add(jsonObj, "maxy", json_object_new_double(_maxY));

    return jsonObj;
}



SymShape* SymRectangle::clone() const {
    SymRectangle* c = new SymRectangle();
    c->_stroke = _stroke->clone();
    c->_fill = _fill->clone();
    c->_minX = _minX;
    c->_minY = _minY;
    c->_maxX = _maxX;
    c->_maxY = _maxY;
    return c;
}

double SymRectangle::maxX() const
{
    return _maxX;
}

double SymRectangle::minX() const
{
    return _minX;
}

double SymRectangle::minY() const
{
    return _minY;
}

double SymRectangle::maxY() const
{
    return _maxY;
}


