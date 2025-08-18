#ifndef __SYM_SYSTEM_FILL_H__
#define __SYM_SYSTEM_FILL_H__

#include "SymShape.h"

class SymSystemFill : public SymShape {
    friend class SymCanvas;
public:
    SymSystemFill();
    virtual ~SymSystemFill();

    bool fromJson(json_object* jsonobj) override;
    json_object* toJson() const override;

    virtual SymShape* clone() const override;
    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
};


#endif
