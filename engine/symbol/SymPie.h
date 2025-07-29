#ifndef __SYM_PIE_H__
#define __SYM_PIE_H__

#include "SymChord.h"

class SymPie : public SymChord {
    friend class SymCanvas;
public:
    SymPie();
    virtual ~SymPie();

    virtual bool fromJson(json_object* jsonobj) override;
    virtual json_object* toJson() const override;

    virtual SymShape* clone() const override;

    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
};


#endif
