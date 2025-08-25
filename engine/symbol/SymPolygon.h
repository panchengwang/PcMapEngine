#ifndef __SYM_POLYGON_H__
#define __SYM_POLYGON_H__


#include <vector>
#include <json-c/json.h>
#include "SymShape.h"
#include "SymPoint.h"
#include "SymLineString.h"

class SymPolygon : public SymLineString
{
    friend class SymCanvas;
public:
    SymPolygon(/* args */);
    virtual ~SymPolygon();

    bool fromJson(json_object* json) override;
    virtual json_object* toJson() const override;

    virtual SymShape* clone() const override;

    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
protected:

};


#endif
