#include "MapSymbol.h"

#include <iostream>
#include "SymShape.h"
#include "SymCircle.h"
#include "JsonUtils.h"
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


MapSymbol::MapSymbol() {

}


MapSymbol::~MapSymbol() {

}

std::string MapSymbol::getErrorMessage() const {
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

std::string MapSymbol::toJson() const {
    json_object* jsonObj = json_object_new_object();
    json_object_object_add(jsonObj, "width", json_object_new_double(_width));
    json_object_object_add(jsonObj, "height", json_object_new_double(_height));
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



sk_sp<SkImage> MapSymbol::createImage(double dotsPerMM) const {
    // std::cerr << "createImage" << std::endl;
    SymCanvas canvas;
    canvas.set(_width, _height, dotsPerMM);
    canvas.begin();
    for (size_t i = 0; i < _shapes.size(); i++) {
        canvas.draw(_shapes[i]);
    }
    canvas.end();
    return canvas.getImage();
}


MapSymbol* MapSymbol::clone() const {
    MapSymbol* ret = new MapSymbol();
    ret->_width = _width;
    ret->_height = _height;
    ret->_dotsPerMM = _dotsPerMM;
    for (size_t i = 0; i < _shapes.size(); i++) {
        ret->_shapes.push_back(_shapes[i]->clone());
    }
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

