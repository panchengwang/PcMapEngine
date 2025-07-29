#ifndef __SYM_CHORD_H__
#define __SYM_CHORD_H__

#include "SymArc.h"

class SymCanvas;
class SymChord : public SymArc {
    friend class SymCanvas;
public:
    SymChord();
    virtual ~SymChord();
    virtual bool fromJson(json_object* jsonobj)  override;
    virtual json_object* toJson() const  override;

    virtual SymShape* clone() const  override;
    size_t memsize() const  override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;

protected:

};

#endif
