#ifndef __SYM_ARC_H__
#define __SYM_ARC_H__

#include "SymShape.h"
#include "SymPoint.h"

class SymCanvas;
class SymArc : public SymShape
{
    friend class SymCanvas;
public:
    SymArc();
    virtual ~SymArc();
    virtual bool fromJson(json_object* jsonobj) override;
    virtual json_object* toJson() const override;

    virtual SymShape* clone() const override;
    double radiusX() const;
    double radiusY() const;
    SymPoint center() const;
    double rotation() const;
    double startAngle() const;
    double endAngle() const;

    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
protected:
    SymPoint _center;
    double _radiusX;
    double _radiusY;
    double _rotation;
    double _startAngle;
    double _endAngle;
};



#endif
