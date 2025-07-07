#ifndef __SYM_FILL_H__
#define __SYM_FILL_H__

#include <string>
#include <SkPaint.h>
#include "SymColor.h"

class SymFill {
public:
    enum FillType {
        FILL_NONE,
        FILL_SOLID,     // 纯色填充
        FILL_GRADIENT,  // 渐变色填充
        FILL_IMAGE,     // 图片填充
    };
public:
    SymFill() {}
    virtual ~SymFill() {}

    virtual bool fromJson(json_object* json, std::string& errMsg) = 0;
    virtual json_object* toJson() const;

    virtual SkPaint toPaint(double dotsPerMM) const = 0;
    virtual SymFill* clone() const = 0;
protected:
    FillType _type;
};



#endif
