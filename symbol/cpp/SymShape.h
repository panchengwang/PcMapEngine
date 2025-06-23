#ifndef __SYM_SHAPE_H
#define __SYM_SHAPE_H

#include <map_engine_dll_exports.h>
// #include <boost/json.hpp>
#include <json.h>
#include <string>
#include "SymStroke.h"

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

    const std::string& getErrorMessage() const;

protected:
    bool strokeFromJson(json_object* jsonobj);

protected:
    int _type;
    std::string _errorMessage;

    SymStroke _stroke;
};


#endif
