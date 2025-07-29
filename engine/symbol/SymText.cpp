#include "SymText.h"
#include "../JsonUtils.h"
#include "../Serialize.h"


SymText::SymText() {
    _type = SYM_TEXT;

}

SymText::~SymText() {

}

bool SymText::fromJson(json_object* jsonobj) {
    if (!SymShape::strokeFromJson(jsonobj)) {
        return false;
    }
    if (!SymShape::fillFromJson(jsonobj)) {
        return false;
    }

    json_object* centerObj;
    if (!json_object_object_get_ex(jsonobj, "center", &centerObj)) {
        _errorMessage = "Missing center in SymText";
        return false;
    }

    if (!_center.fromJson(centerObj, _errorMessage)) {
        return false;
    }

    JSON_GET_DOUBLE(jsonobj, "rotation", _rotation, _errorMessage);
    JSON_GET_STRING(jsonobj, "text", _text, _errorMessage);
    JSON_GET_DOUBLE(jsonobj, "fontsize", _fontSize, _errorMessage);
    JSON_GET_STRING(jsonobj, "fontname", _fontName, _errorMessage);
    JSON_GET_BOOLEAN(jsonobj, "outlined", _outlined, _errorMessage);
    JSON_GET_DOUBLE(jsonobj, "outlinedwidth", _outlinedWidth, _errorMessage);

    std::string slant;
    JSON_GET_STRING(jsonobj, "slant", slant, _errorMessage);
    if (slant == "italic") {
        _slant = SLANT_ITALIC;
    }
    else if (slant == "oblique") {
        _slant = SLANT_OBLIQUE;
    }
    else {
        _slant = SLANT_NORMAL;
    }

    std::string weight;
    JSON_GET_STRING(jsonobj, "weight", weight, _errorMessage);
    if (weight == "bold") {
        _weight = WEIGHT_BOLD;
    }
    else {
        _weight = WEIGHT_NORMAL;
    }


    return true;
}


json_object* SymText::toJson() const {
    json_object* jsonObj = SymShape::toJson();
    json_object_object_add(jsonObj, "type", json_object_new_string("text"));
    json_object_object_add(jsonObj, "stroke", _stroke->toJson());
    json_object_object_add(jsonObj, "fill", _fill->toJson());
    json_object_object_add(jsonObj, "center", _center.toJson());
    json_object_object_add(jsonObj, "rotation", json_object_new_double(_rotation));
    json_object_object_add(jsonObj, "text", json_object_new_string(_text.c_str()));
    json_object_object_add(jsonObj, "fontsize", json_object_new_double(_fontSize));
    json_object_object_add(jsonObj, "fontname", json_object_new_string(_fontName.c_str()));
    json_object_object_add(jsonObj, "outlined", json_object_new_boolean(_outlined));
    json_object_object_add(jsonObj, "outlinedWidth", json_object_new_double(_outlinedWidth));

    switch (_slant) {
    case SLANT_NORMAL:
        json_object_object_add(jsonObj, "slant", json_object_new_string("normal"));
        break;
    case SLANT_ITALIC:
        json_object_object_add(jsonObj, "slant", json_object_new_string("italic"));
        break;
    case SLANT_OBLIQUE:
        json_object_object_add(jsonObj, "slant", json_object_new_string("oblique"));
        break;
    }

    switch (_weight) {
    case WEIGHT_NORMAL:
        json_object_object_add(jsonObj, "weight", json_object_new_string("normal"));
        break;
    case WEIGHT_BOLD:
        json_object_object_add(jsonObj, "weight", json_object_new_string("bold"));
        break;
    }
    return jsonObj;
}



SymShape* SymText::clone() const {
    SymText* shp = new SymText();
    shp->_type = _type;
    shp->_stroke = _stroke->clone();
    shp->_fill = _fill->clone();
    shp->_text = _text;
    shp->_center = _center;
    shp->_fontSize = _fontSize;
    shp->_fontName = _fontName;
    shp->_rotation = _rotation;
    shp->_outlined = _outlined;
    shp->_outlinedWidth = _outlinedWidth;
    return shp;
}

size_t SymText::memsize() const {
    size_t size = 0;
    size += sizeof(_type);
    size += _stroke->memsize();
    size += _fill->memsize();
    // size += sizeof(int);            // for _text length
    size += _text.length() + 1;           // for _text
    size += _center.memsize();          // for _center
    size += sizeof(_fontSize);         // for _fontSize
    size += _fontName.length() + 1;       // for _fontName
    size += sizeof(_rotation);        // for _rotation
    size += sizeof(_outlined);        // for _outlined
    size += sizeof(_outlinedWidth);        // for _outlined
    size += sizeof(_weight);        // for _weight
    size += sizeof(_slant);        // for _slant
    return size;
}


char* SymText::serialize(char* p) const {
    SERIALIZE(p, _type);
    p = _stroke->serialize(p);
    p = _fill->serialize(p);
    SERIALIZE_STRING(p, _text);
    p = _center.serialize(p);
    SERIALIZE(p, _fontSize);
    SERIALIZE_STRING(p, _fontName);
    SERIALIZE(p, _rotation);
    SERIALIZE(p, _outlined);
    SERIALIZE(p, _outlinedWidth);
    SERIALIZE(p, _weight);
    SERIALIZE(p, _slant);
    return p;
}

char* SymText::deserialize(char* data) {
    char* p = data;
    DESERIALIZE(p, _type);
    p = _stroke->deserialize(p);
    p = fillDeserialize(p);
    DESERIALIZE_STRING(p, _text);
    p = _center.deserialize(p);
    DESERIALIZE(p, _fontSize);
    DESERIALIZE_STRING(p, _fontName);
    DESERIALIZE(p, _rotation);
    DESERIALIZE(p, _outlined);
    DESERIALIZE(p, _outlinedWidth);
    DESERIALIZE(p, _weight);
    DESERIALIZE(p, _slant);
    return p;
}