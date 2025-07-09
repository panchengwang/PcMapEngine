#include "SymStar.h"
#include "JsonUtils.h"
#include <cmath>
#include <math.h>

SymStar::SymStar() {
    _type = SYM_STAR;
    _center = SymPoint(0, 0);
    _radius = 0.5;
    _radius2 = 0.2;
    _sides = 4;
    _rotation = 0;
}


SymStar::~SymStar() {

}


bool SymStar::fromJson(json_object* jsonobj) {
    if (!SymRegularPolygon::fromJson(jsonobj)) {
        return false;
    }
    JSON_GET_DOUBLE(jsonobj, "radius2", _radius2, _errorMessage)
        return true;
}


json_object* SymStar::toJson() const {
    json_object* jsonObj = SymRegularPolygon::toJson();
    json_object_object_add(jsonObj, "type", json_object_new_string("star"));
    json_object_object_add(jsonObj, "radius2", json_object_new_double(_radius2));
    return jsonObj;
}



SymShape* SymStar::clone() const {
    SymStar* rpg = new SymStar();
    rpg->_type = _type;
    rpg->_stroke = _stroke->clone();
    rpg->_fill = _fill->clone();
    rpg->_center = _center;
    rpg->_radius = _radius;
    rpg->_radius2 = _radius2;
    rpg->_sides = _sides;
    rpg->_rotation = _rotation;
    return rpg;
}



std::vector<SymPoint> SymStar::getPoints() const {
    std::vector<SymPoint> points;
    double step = M_PI / _sides;
    double angle = 0;
    angle += M_PI * 0.5;
    double r, x, y;
    for (size_t i = 0; i < 2 * _sides; ++i) {
        r = (i % 2 == 0) ? _radius : _radius2;
        x = _center.x() + r * cos(angle);
        y = _center.y() + r * sin(angle);
        points.push_back(SymPoint(x, y));
        angle += step;
    }
    return points;
}

