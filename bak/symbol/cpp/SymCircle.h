#ifndef __SYM_CIRCLE_H
#define __SYM_CIRCLE_H

#include "SymShape.h"
#include "SymPoint.h"


class SymCircle : public SymShape
{
    friend class SymCanvas;
public:
    SymCircle();
    virtual ~SymCircle();

    virtual bool fromJson(json_object* jsonObj) override;
    virtual json_object* toJson() const override;
    virtual SymShape* clone() const override;

    double radius() const { return _radius; }
    void setRadius(double r) { _radius = r; }

    SymPoint center() const { return _center; }
    void setCenter(const SymPoint& c) { _center = c; }

    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
protected:

    double _radius;
    SymPoint _center;
};


#endif
