#ifndef __SYM_RECTANGLE_H__
#define __SYM_RECTANGLE_H__

#include "SymShape.h"


class SymRectangle : public SymShape {
    friend class SymCanvas;
public:
    SymRectangle();
    virtual ~SymRectangle();

    virtual bool fromJson(json_object* jsonobj) override;
    virtual json_object* toJson() const override;

    virtual SymShape* clone() const override;

    double maxX() const;
    double minX() const;
    double minY() const;
    double maxY() const;
    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
protected:

    double _minX, _minY, _maxX, _maxY;
};

#endif
