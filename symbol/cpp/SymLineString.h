#ifndef __SYM_LINESTRING_H__
#define __SYM_LINESTRING_H__


#include <vector>
#include <json-c/json.h>
#include "SymShape.h"
#include "SymPoint.h"

class SymLineString : public SymShape
{
public:
    SymLineString(/* args */);
    virtual ~SymLineString();

    bool fromJson(json_object* json);
    virtual json_object* toJson() const;

    virtual SymShape* clone() const;

    const std::vector<SymPoint>& points() const;
protected:
    std::vector<SymPoint> _points;
};


#endif
