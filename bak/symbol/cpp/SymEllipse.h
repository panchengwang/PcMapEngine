#ifndef __SYM_ELLIPSE_H__
#define __SYM_ELLIPSE_H__

#include "SymShape.h"
#include "SymPoint.h"

class SymEllipse : public SymShape
{
    friend class SymCanvas;
public:
    SymEllipse();
    virtual ~SymEllipse();
    virtual bool fromJson(json_object* jsonobj) override;
    virtual json_object* toJson() const override;

    virtual SymShape* clone() const override;
    double radiusX() const;
    double radiusY() const;
    SymPoint center() const;
    double rotation() const;
    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
protected:
    SymPoint _center;
    double _radiusX;
    double _radiusY;
    double _rotation;
};

#endif
