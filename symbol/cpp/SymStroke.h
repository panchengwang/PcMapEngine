#ifndef __SYM_STROKE_H__
#define __SYM_STROKE_H__

#include <json.h>
#include <string>
#include <vector>
#include "SymColor.h"

class SymStroke
{
public:
    enum CapStyle {
        CAP_BUTT,
        CAP_ROUND,
        CAP_SQUARE
    };

    enum JoinStyle {
        JOIN_MITER,
        JOIN_ROUND,
        JOIN_BEVEL
    };

public:
    SymStroke(void);
    ~SymStroke(void);


    bool fromJson(json_object* jsonObj, std::string& errMsg);
    json_object* toJson();


protected:
    CapStyle _cap;
    JoinStyle _join;
    std::vector<double> _dashes;
    SymColor _color;

};


#endif