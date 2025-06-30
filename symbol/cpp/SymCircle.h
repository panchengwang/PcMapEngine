#ifndef __SYM_CIRCLE_H
#define __SYM_CIRCLE_H

#include "SymShape.h"
#include "SymPoint.h"

class SymCircle : public SymShape
{
public:
    SymCircle();
    virtual ~SymCircle();

    virtual bool fromJson(json_object* jsonObj);
    virtual json_object* toJson() const;

    double radius() const { return _radius; }
    void setRadius(double r) { _radius = r; }

    SymPoint center() const { return _center; }
    void setCenter(const SymPoint& c) { _center = c; }
protected:

    double _radius;
    SymPoint _center;
};


#endif
