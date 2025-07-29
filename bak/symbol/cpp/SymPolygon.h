#ifndef __SYM_POLYGON_H__
#define __SYM_POLYGON_H__


#include <vector>
#include <json-c/json.h>
#include "SymShape.h"
#include "SymPoint.h"

class SymPolygon : public SymShape
{
    friend class SymCanvas;
public:
    SymPolygon(/* args */);
    virtual ~SymPolygon();

    bool fromJson(json_object* json) override;
    virtual json_object* toJson() const override;

    virtual SymShape* clone() const override;

    const std::vector<SymPoint>& points() const;
    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
protected:
    std::vector<SymPoint> _points;
};


#endif
