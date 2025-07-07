#ifndef __SYM_SOLID_FILL_H__
#define __SYM_SOLID_FILL_H__

#include "SymFill.h"

class SymSolidFill : public SymFill {
public:
    SymSolidFill();
    virtual ~SymSolidFill() {}
    bool fromJson(json_object* json, std::string& errMsg) override;
    json_object* toJson() const override;
    SymFill* clone() const override;
    virtual SkPaint toPaint(double dotsPerMM) const override;
private:
    SymColor _color;
};



#endif
