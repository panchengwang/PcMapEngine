#ifndef __SYM_STROKE_H__
#define __SYM_STROKE_H__

#include <json-c/json.h>
#include <string>
#include <vector>
#include "SymColor.h"
#include <SkPaint.h>

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
    SymStroke();
    SymStroke(const SymStroke& stroke);

    SymStroke* clone() const;
    ~SymStroke();


    bool fromJson(json_object* jsonObj, std::string& errMsg);
    json_object* toJson() const;

    SkPaint toPaint(double dotsPerMM) const;

protected:
    CapStyle _cap;
    JoinStyle _join;
    std::vector<double> _dashes;
    SymColor _color;
    double _width;

};


#endif
