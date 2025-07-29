#ifndef __SYM_REGULAR_POLYGON_H__
#define __SYM_REGULAR_POLYGON_H__


#include "SymShape.h"
#include "SymPoint.h"

class SymRegularPolygon : public SymShape
{
    friend class SymCanvas;
public:
    SymRegularPolygon();
    virtual ~SymRegularPolygon();
    virtual bool fromJson(json_object* jsonobj) override;
    virtual json_object* toJson() const override;

    virtual SymShape* clone() const override;

    std::vector<SymPoint> getPoints() const;

    SymPoint center() const { return _center; }
    void setCenter(const SymPoint& pt) { _center = pt; }

    double radius() const { return _radius; }
    void setRadius(double r) { _radius = r; }

    int sides() const { return _sides; }
    void setSides(int s) { _sides = s; }

    double rotation() const { return _rotation; }
    void setRotation(double r) { _rotation = r; }

    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
protected:
    SymPoint _center;
    double _radius;
    int _sides;
    double _rotation;
};


#endif
