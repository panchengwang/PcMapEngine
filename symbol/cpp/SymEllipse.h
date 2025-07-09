#ifndef __SYM_ELLIPSE_H__
#define __SYM_ELLIPSE_H__

#include "SymShape.h"
#include "SymPoint.h"

class SymEllipse : public SymShape
{
public:
    SymEllipse();
    virtual ~SymEllipse();
    virtual bool fromJson(json_object* jsonobj);
    virtual json_object* toJson() const;

    virtual SymShape* clone() const;
    double radiusX() const;
    double radiusY() const;
    SymPoint center() const;
    double rotation() const;

protected:
    SymPoint _center;
    double _radiusX;
    double _radiusY;
    double _rotation;
};

#endif
