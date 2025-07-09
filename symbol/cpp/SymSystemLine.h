#ifndef __SYM_SYSTEM_LINE_H__
#define __SYM_SYSTEM_LINE_H__

#include "SymShape.h"

class SymSystemLine : public SymShape {
public:
    SymSystemLine();
    virtual ~SymSystemLine();

    virtual bool fromJson(json_object* jsonobj);
    virtual json_object* toJson() const;

    virtual SymShape* clone() const;

};


#endif
