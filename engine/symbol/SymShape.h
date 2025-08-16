#ifndef __SYM_SHAPE_H
#define __SYM_SHAPE_H

#include <map_engine_dll_exports.h>
// #include <boost/json.hpp>
#include <json-c/json.h>
#include <string>
#include "SymStroke.h"
#include "SymFill.h"

class SymCanvas;
class SymShape
{
    friend class SymCanvas;
public:
    enum SHAPE_TYPE {
        SYM_NONE = 0,
        SYM_SYSTEM_LINE,
        SYM_LINESTRING,
        SYM_POLYGON,
        SYM_REGULAR_POLYGON,
        SYM_RECTANGLE,
        SYM_CIRCLE,
        SYM_ELLIPSE,
        SYM_ARC,
        SYM_PIE,
        SYM_CHORD,
        SYM_STAR,
        SYM_TEXT
    };


    virtual size_t memsize() const = 0;
public:
    SymShape();
    virtual ~SymShape();


    uint8_t type() const;

    virtual bool fromJson(json_object* jsonobj);
    virtual json_object* toJson() const;

    virtual SymShape* clone() const = 0;
    const std::string& getErrorMessage() const;

    const SymStroke* stroke() const { return _stroke; }
    const SymFill* fill() const { return _fill; }
    virtual char* serialize(char* p) const = 0;
    virtual char* deserialize(char* data) = 0;
protected:
    bool strokeFromJson(json_object* jsonobj);
    bool fillFromJson(json_object* jsonobj);

    char* fillDeserialize(char* data);


protected:
    uint8_t _type;
    std::string _errorMessage;

    SymStroke* _stroke;
    SymFill* _fill;
};


#endif
