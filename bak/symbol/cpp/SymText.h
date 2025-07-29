#ifndef __SYM_TEXT_H__
#define __SYM_TEXT_H__

#include "SymShape.h"
#include "SymPoint.h"
#include <string>


class SymText : public SymShape
{
    friend class SymCanvas;
public:
    enum WeightStyle {
        WEIGHT_NORMAL,
        WEIGHT_BOLD
    };

    enum SlantStyle {
        SLANT_NORMAL,
        SLANT_ITALIC,
        SLANT_OBLIQUE
    };

    SymText();
    virtual ~SymText();

    bool fromJson(json_object* jsonobj) override;
    json_object* toJson() const override;
    SymShape* clone() const override;
    size_t memsize() const override;
    char* serialize(char* p) const override;
    char* deserialize(char* data) override;
protected:
    std::string _text;
    SymPoint _center;
    double _fontSize;
    std::string _fontName;
    double _rotation;
    bool _outlined;
    double _outlinedWidth;
    uint8_t _weight;
    uint8_t _slant;
};



#endif
