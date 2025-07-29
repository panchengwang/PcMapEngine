#ifndef __SYM_SYSTEM_LINE_H__
#define __SYM_SYSTEM_LINE_H__

#include "SymShape.h"

class SymSystemLine : public SymShape {
    friend class SymCanvas;
public:
    SymSystemLine();
    virtual ~SymSystemLine();

     bool fromJson(json_object* jsonobj) override;
     json_object* toJson() const override;

    virtual SymShape* clone() const override;
    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
};


#endif
