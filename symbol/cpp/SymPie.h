#ifndef __SYM_PIE_H__
#define __SYM_PIE_H__

#include "SymChord.h"

class SymPie : public SymChord {
public:
    SymPie();
    virtual ~SymPie();

    virtual bool fromJson(json_object* jsonobj);
    virtual json_object* toJson() const;

    virtual SymShape* clone() const;
};


#endif
