#ifndef __SYM_STAR_H__
#define __SYM_STAR_H__


#include "SymShape.h"
#include "SymPoint.h"
#include "SymRegularPolygon.h"

class SymStar : public SymRegularPolygon
{
public:
    SymStar();
    virtual ~SymStar();
    virtual bool fromJson(json_object* jsonobj);
    virtual json_object* toJson() const;

    virtual SymShape* clone() const;

    std::vector<SymPoint> getPoints() const;


protected:
    double _radius2;
};


#endif