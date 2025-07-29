#ifndef __SYM_SOLID_FILL_H__
#define __SYM_SOLID_FILL_H__

#include "SymFill.h"

class SymSolidFill : public SymFill {
    friend class SymCanvas;
public:
    SymSolidFill();
    virtual ~SymSolidFill() {}
    bool fromJson(json_object* json, std::string& errMsg) override;
    json_object* toJson() const override;
    SymFill* clone() const override;
    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
private:
    SymColor _color;
};



#endif
