#ifndef __SYM_RECTANGLE_H__
#define __SYM_RECTANGLE_H__

#include "SymShape.h"


class SymRectangle : public SymShape {
public:
    SymRectangle();
    virtual ~SymRectangle();

    virtual bool fromJson(json_object* jsonobj);
    virtual json_object* toJson() const;

    virtual SymShape* clone() const;

    double maxX() const;
    double minX() const;
    double minY() const;
    double maxY() const;

protected:

    double _minX, _minY, _maxX, _maxY;
};

#endif
