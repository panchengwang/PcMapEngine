#ifndef __SYM_REGULAR_POLYGON_H__
#define __SYM_REGULAR_POLYGON_H__


#include "SymShape.h"
#include "SymPoint.h"

class SymRegularPolygon : public SymShape
{
public:
    SymRegularPolygon();
    virtual ~SymRegularPolygon();
    virtual bool fromJson(json_object* jsonobj);
    virtual json_object* toJson() const;

    virtual SymShape* clone() const;

    std::vector<SymPoint> getPoints() const;

    SymPoint center() const { return _center; }
    void setCenter(const SymPoint& pt) { _center = pt; }

    double radius() const { return _radius; }
    void setRadius(double r) { _radius = r; }

    int sides() const { return _sides; }
    void setSides(int s) { _sides = s; }

    double rotation() const { return _rotation; }
    void setRotation(double r) { _rotation = r; }
protected:
    SymPoint _center;
    double _radius;
    int _sides;
    double _rotation;
};


#endif