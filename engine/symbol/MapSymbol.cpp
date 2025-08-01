#include "MapSymbol.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include "../JsonUtils.h"
#include "../Serialize.h"
#include "SymShape.h"
#include "SymCircle.h"
#include "SymCanvas.h"
#include "SymRectangle.h"
#include "SymPolygon.h"
#include "SymLineString.h"
#include "SymEllipse.h"
#include "SymArc.h"
#include "SymChord.h"
#include "SymPie.h"
#include "SymSystemLine.h"
#include "SymRegularPolygon.h"
#include "SymStar.h"
#include "SymText.h"



MapSymbol::MapSymbol() {
    _dotsPerMM = 96.0 / 25.4;
}


MapSymbol::~MapSymbol() {

}

const std::string& MapSymbol::getErrorMessage() const {
    return _errorMessage;
}



bool MapSymbol::fromJson(const std::string& jsonstr) {
    bool ret = true;
    json_object* jsonObj = json_tokener_parse(jsonstr.c_str());

    if (!jsonObj) {
        _errorMessage = "Invalid JSON string";
        return false;
    }
    ret = fromJson(jsonObj);
    return ret;
}



bool MapSymbol::fromJson(json_object* jsonObj) {
    clear();

    JSON_GET_DOUBLE(jsonObj, "width", _width, _errorMessage);
    JSON_GET_DOUBLE(jsonObj, "height", _height, _errorMessage);
    JSON_GET_DOUBLE(jsonObj, "dotspermm", _dotsPerMM, _errorMessage);
    json_object* shapesObj;
    JSON_GET_ARRAY(jsonObj, "shapes", shapesObj, _errorMessage);

    for (size_t i = 0; i < json_object_array_length(shapesObj); i++) {
        json_object* shapeObj = json_object_array_get_idx(shapesObj, i);
        std::string typestr;
        JSON_GET_STRING(shapeObj, "type", typestr, _errorMessage);
        SymShape* shape = NULL;
        if (typestr == "circle") {
            shape = new SymCircle();
        }
        else if (typestr == "rectangle") {
            shape = new SymRectangle();
        }
        else if (typestr == "polygon") {
            shape = new SymPolygon();
        }
        else if (typestr == "linestring") {
            shape = new SymLineString();
        }
        else if (typestr == "ellipse") {
            shape = new SymEllipse();
        }
        else if (typestr == "arc") {
            shape = new SymArc();
        }
        else if (typestr == "chord") {
            shape = new SymChord();
        }
        else if (typestr == "pie") {
            shape = new SymPie();
        }
        else if (typestr == "systemline") {
            shape = new SymSystemLine();
        }
        else if (typestr == "regularpolygon") {
            shape = new SymRegularPolygon();
        }
        else if (typestr == "star") {
            shape = new SymStar();
        }
        else if (typestr == "text")
        {
            shape = new SymText();
        }
        else {
            _errorMessage = std::string("Unsupported shape type: ") + typestr;
            return false;
        }

        if (shape != NULL && !shape->fromJson(shapeObj)) {
            _errorMessage = shape->getErrorMessage();
            delete shape;
            clear();
            return false;
        }

        _shapes.push_back(shape);
    }



    return true;
}


bool MapSymbol::fromJsonFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        _errorMessage = "Failed to open file: " + std::string(filename);
        return false;
    }

    // 获取文件大小
    auto fileSize = std::filesystem::file_size(filename);
    std::string content;
    content.resize(fileSize);

    // 读取整个文件
    file.read(content.data(), fileSize);
    return fromJson(content);
}


std::string MapSymbol::toJson() const {
    json_object* jsonObj = json_object_new_object();
    json_object_object_add(jsonObj, "width", json_object_new_double(_width));
    json_object_object_add(jsonObj, "height", json_object_new_double(_height));
    json_object_object_add(jsonObj, "dotspermm", json_object_new_double(_dotsPerMM));

    json_object* shapesArr = json_object_new_array();

    for (size_t i = 0; i < _shapes.size(); i++) {
        json_object* shapeObj = _shapes[i]->toJson();
        json_object_array_add(shapesArr, shapeObj);
    }
    json_object_object_add(jsonObj, "shapes", shapesArr);
    std::string jsonstr = json_object_to_json_string(jsonObj);
    json_object_put(jsonObj);
    return jsonstr;
}


// void MapSymbol::toJson(boost::json::value& jsonValue) const {
//     boost::json::object& obj = jsonValue.as_object();
//     obj["width"] = _width;
//     obj["height"] = _height;
//     if (_shapes.size() > 0) {
//         boost::json::array arr;
//         for (size_t i = 0; i < _shapes.size(); i++) {
//             arr.push_back(_shapes[i]->toJson());
//         }
//         obj["shapes"] = arr;
//     }
// }

void MapSymbol::clear() {
    for (size_t i = 0; i < _shapes.size(); i++) {
        delete _shapes[i];
    }

    _shapes.clear();
}




/**
 * 获取符号的图像数据。
 * 返回的数据请务必使用free()释放内存，不要使用delete释放内存。
 *
 */
char* MapSymbol::imageData(size_t& size) const
{
    SymCanvas canvas;
    canvas.set(_width, _height, _dotsPerMM);
    canvas.begin();
    for (size_t i = 0; i < _shapes.size(); i++) {
        canvas.draw(_shapes[i]);
    }
    canvas.end();
    return canvas.data(size);
}


MapSymbol* MapSymbol::clone() const {
    MapSymbol* ret = new MapSymbol();
    ret->_width = _width;
    ret->_height = _height;
    ret->_dotsPerMM = _dotsPerMM;
    for (size_t i = 0; i < _shapes.size(); i++) {
        ret->_shapes.push_back(_shapes[i]->clone());
    }
    return ret;
}


MapSymbol& MapSymbol::operator=(const MapSymbol& other) {
    clear();
    _width = other._width;
    _height = other._height;
    for (size_t i = 0; i < other._shapes.size(); i++) {
        _shapes.push_back(other._shapes[i]->clone());
    }

    return *this;
}



size_t MapSymbol::memsize() const {
    size_t size = 0;
    size += sizeof(_width);
    size += sizeof(_height);
    size += sizeof(_dotsPerMM);
    size += sizeof(int32_t);
    for (size_t i = 0; i < _shapes.size(); i++) {
        size += _shapes[i]->memsize();
    }
    return size;
}


char* MapSymbol::serialize(size_t& len) const {
    len = memsize();
    char* ret = new char[len];
    memset(ret, 0, len);
    char* p = ret;
    SERIALIZE(p, _width);
    SERIALIZE(p, _height);
    SERIALIZE(p, _dotsPerMM);
    int32_t nshapes = (int32_t)_shapes.size();
    SERIALIZE(p, nshapes);
    for (size_t i = 0; i < _shapes.size(); i++) {
        p = _shapes[i]->serialize(p);
    }

    return ret;
}

bool MapSymbol::deserialize(char* data) {
    char* p = data;
    DESERIALIZE(p, _width);
    DESERIALIZE(p, _height);
    DESERIALIZE(p, _dotsPerMM);
    int32_t nshapes;
    DESERIALIZE(p, nshapes);
    clear();
    uint8_t type;
    SymShape* shp = NULL;
    for (size_t i = 0; i < nshapes; i++) {
        memcpy(&type, p, sizeof(uint8_t));                   // do not use SERIALIZE macro here
        switch (type) {
        case SymShape::SYM_ARC:
            shp = new SymArc();
            break;
        case SymShape::SYM_CHORD:
            shp = new SymChord();
            break;
        case SymShape::SYM_PIE:
            shp = new SymPie();
            break;
        case SymShape::SYM_REGULAR_POLYGON:
            shp = new SymRegularPolygon();
            break;
        case SymShape::SYM_STAR:
            shp = new SymStar();
            break;
        case SymShape::SYM_LINESTRING:
            shp = new SymLineString();
            break;
        case SymShape::SYM_ELLIPSE:
            shp = new SymEllipse();
            break;
        case SymShape::SYM_CIRCLE:
            shp = new SymCircle();
            break;
        case SymShape::SYM_RECTANGLE:
            shp = new SymRectangle();
            break;
        case SymShape::SYM_POLYGON:
            shp = new SymPolygon();
            break;
        case SymShape::SYM_SYSTEM_LINE:
            shp = new SymSystemLine();
            break;
        case SymShape::SYM_TEXT:
            shp = new SymText();
            break;
        default:
            break;
        }
        if (!shp) {
            _errorMessage = "Invalid shape type";
            return false;
        }
        p = shp->deserialize(p);
        _shapes.push_back(shp);
    }

    return true;
}

std::vector<MapSymbol*> MapSymbol::extract() const {
    std::vector<MapSymbol*> ret;
    for (size_t i = 0; i < _shapes.size(); i++) {
        MapSymbol* sym = new MapSymbol();
        sym->_width = _width;
        sym->_height = _height;
        sym->_dotsPerMM = _dotsPerMM;
        sym->_shapes.push_back(_shapes[i]->clone());
        ret.push_back(sym);
    }
    return ret;
}