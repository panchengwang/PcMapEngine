#ifndef __SYM_ARC_H__
#define __SYM_ARC_H__

#include "SymShape.h"
#include "SymPoint.h"

class SymArc : public SymShape
{
public:
    SymArc();
    virtual ~SymArc();
    virtual bool fromJson(json_object* jsonobj);
    virtual json_object* toJson() const;

    virtual SymShape* clone() const;
    double radiusX() const;
    double radiusY() const;
    SymPoint center() const;
    double rotation() const;
    double startAngle() const;
    double endAngle() const;
protected:
    SymPoint _center;
    double _radiusX;
    double _radiusY;
    double _rotation;
    double _startAngle;
    double _endAngle;
};



#endif
