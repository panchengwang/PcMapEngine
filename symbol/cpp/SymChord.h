#ifndef __SYM_CHORD_H__
#define __SYM_CHORD_H__

#include "SymArc.h"

class SymChord : public SymArc {
public:
    SymChord();
    virtual ~SymChord();
    virtual bool fromJson(json_object* jsonobj);
    virtual json_object* toJson() const;

    virtual SymShape* clone() const;
protected:

};

#endif