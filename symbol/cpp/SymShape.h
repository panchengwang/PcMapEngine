#ifndef __SYM_SHAPE_H
#define __SYM_SHAPE_H

#include <map_engine_dll_exports.h>
// #include <boost/json.hpp>
#include <json-c/json.h>
#include <string>
#include "SymStroke.h"
#include "SymFill.h"

class SymShape
{
public:
    enum {
        SYM_NONE = 0,
        SYM_SYS_LINE,
        SYM_POLYGON,
        SYM_REGULAR_POLYGON,
        SYM_RECTANGLE,
        SYM_CIRCLE,
        SYM_ELLIPSE,
        SYM_ARC,
        SYM_PIE,
        SYM_CHORD,
        SYM_STAR
    };



public:
    SymShape();
    virtual ~SymShape();


    int type() const;

    virtual bool fromJson(json_object* jsonobj);
    virtual json_object* toJson() const;

    virtual SymShape* clone() const = 0;
    const std::string& getErrorMessage() const;

    const SymStroke* stroke() const { return _stroke; }
    const SymFill* fill() const { return _fill; }
protected:
    bool strokeFromJson(json_object* jsonobj);
    bool fillFromJson(json_object* jsonobj);
protected:
    int _type;
    std::string _errorMessage;

    SymStroke* _stroke;
    SymFill* _fill;
};


#endif
