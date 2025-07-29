#ifndef __SYM_STAR_H__
#define __SYM_STAR_H__


#include "SymShape.h"
#include "SymPoint.h"
#include "SymRegularPolygon.h"

class SymStar : public SymRegularPolygon
{
    friend class SymCanvas;
public:
    SymStar();
    virtual ~SymStar();
    virtual bool fromJson(json_object* jsonobj) override;
    virtual json_object* toJson() const override;

    virtual SymShape* clone() const override;

    std::vector<SymPoint> getPoints() const;

    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
protected:
    double _radius2;
};


#endif
