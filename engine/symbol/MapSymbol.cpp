#include "MapSymbol.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include "JsonUtils.h"
#include "Serialize.h"
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
#include "SymSystemFill.h"
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


/**
 * @brief Load a MapSymbol from a JSON object.
 * This function populates the MapSymbol object with properties and shapes from the provided JSON object.
 * @param jsonObj The JSON object containing the MapSymbol data.
 * @return True if the JSON object was successfully parsed and the MapSymbol was populated, false otherwise.
 * If the JSON object is invalid or missing required fields, an error message is set. Use getErrorMessage() to retrieve it.
 * @note The JSON object should contain the following fields:
 * - "width": The width of the symbol.
 * - "height": The height of the symbol.
 * - "dotspermm": The dots per millimeter for the symbol.
 * - "shapes": An array of shape objects, each containing a "type" field
 */
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
        else if (typestr == "systemfill") {
            shape = new SymSystemFill();
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


/**
 * @brief Load a MapSymbol from a JSON file.
 * This function reads a JSON file and populates the MapSymbol object with its contents.
 * @param filename The path to the JSON file.
 * @return True if the file was successfully read and parsed, false otherwise.
 * If the file cannot be opened or parsed, an error message is set . Using getErrorMessage() to retrieve it.
 * @note The JSON file should contain a valid MapSymbol representation.
 */
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


/**
 * @brief Convert the MapSymbol object to a JSON string.
 * This function creates a JSON object representing the MapSymbol's properties and shapes.
 * @return A string containing the JSON representation of the MapSymbol.
 */
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


/**
 * @brief Clear the MapSymbol object.
 * This function deletes all shapes in the symbol and clears the shape vector.
 */
void MapSymbol::clear() {
    for (size_t i = 0; i < _shapes.size(); i++) {
        delete _shapes[i];
    }

    _shapes.clear();
}




/**
 * @brief Get the image data of the symbol.
 * The returned data must be freed using free(), not delete.
 * @param size The size of the image data in bytes.
 * @return Pointer to the image data. Returns NULL if there is no image data.
 * This function creates a SymCanvas, draws all shapes onto it, and retrieves the image data.
 * The image data is returned as a dynamically allocated char array, use free() to release it.
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





/**
 * @brief Get a copy of the cairo surface for the MapSymbol.
 * This function creates a new SymCanvas, draws all shapes onto it, and returns the cairo surface.
 * The caller is responsible for destroying the returned surface using cairo_surface_destroy().
 * @return A pointer to the cairo surface containing the drawn shapes.
 * The returned surface is a copy of the original surface used in the SymCanvas.
 * The caller must call cairo_surface_destroy() on the returned surface when it is no longer needed.
 */
 // cairo_surface_t* MapSymbol::cairoSurface() const {
 //     SymCanvas canvas;
 //     canvas.set(_width, _height, _dotsPerMM);
 //     canvas.begin();
 //     for (size_t i = 0; i < _shapes.size(); i++) {
 //         canvas.draw(_shapes[i]);
 //     }
 //     canvas.end();
 //     return canvas.cairoSurface();
 // }


 /**
  * @brief Get a copy of the cairo surface for the MapSymbol.
  * This function creates a new SymCanvas, draws all shapes onto it, and returns the cairo surface.
  * The caller is responsible for destroying the returned surface using cairo_surface_destroy().
  * @param withShape Specifies which shapes to include in the surface.
  * - CREATE_SURFACE_WITH_SYSTEM_LINE: Include system line shapes.
  * - CREATE_SURFACE_WITH_SYSTEM_FILL: Include system fill shapes.
  * - CREATE_SURFACE_WITH_ALL_SHAPES: Include all shapes.
  * @return A pointer to the cairo surface containing the drawn shapes.
  * The returned surface is a copy of the original surface used in the SymCanvas.
  * The caller must call cairo_surface_destroy() on the returned surface when it is no longer needed.
  */
cairo_surface_t* MapSymbol::cairoSurface(uint8_t withShape) const {
    SymCanvas canvas;
    canvas.set(_width, _height, _dotsPerMM);
    canvas.begin();
    for (size_t i = 0; i < _shapes.size(); i++) {
        if ((withShape & CREATE_SURFACE_WITHOUT_SYSTEM_LINE) && _shapes[i]->type() == SymShape::SYM_SYSTEM_LINE) {
            continue;
        }
        else if ((withShape & CREATE_SURFACE_WITHOUT_SYSTEM_LINE) && _shapes[i]->type() == SymShape::SYM_SYSTEM_FILL) {
            continue;
        }


        canvas.draw(_shapes[i]);
        std::cerr << "draw shape: " << std::endl;

    }
    canvas.end();
    return canvas.cairoSurface();
}


/**
 * @brief Clone the MapSymbol object.
 *      This creates a new MapSymbol object with the same properties and shapes as the current one.
 * @return A pointer to the newly created MapSymbol object.
 *      The caller is responsible for deleting the returned object.
 */
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
    _dotsPerMM = other._dotsPerMM;
    for (size_t i = 0; i < other._shapes.size(); i++) {
        _shapes.push_back(other._shapes[i]->clone());
    }

    return *this;
}



/**
 * @brief Get the memory size of the MapSymbol object.
 * This function calculates the total memory size used by the MapSymbol object, including its properties and
 * shapes.
 * @return The total memory size in bytes.
 */
size_t MapSymbol::memsize() const {
    size_t size = 0;
    size += sizeof(double);                     // _width
    size += sizeof(double);                     // _height
    size += sizeof(double);                     // _dotsPerMM

    size += sizeof(size_t);
    for (size_t i = 0; i < _shapes.size(); i++) {
        size += _shapes[i]->memsize();
    }
    return size;
}


/**
 * @brief Serialize the MapSymbol object to a byte array.
 * This function converts the MapSymbol's properties and shapes into a byte array for storage or transmission.
 * @param len The length of the serialized data in bytes.
 * @return A pointer to the serialized data. The caller is responsible for freeing this memory.
 * The serialized data includes the width, height, dots per mm, number of shapes, and each shape's serialized data.
 */
char* MapSymbol::serialize(size_t& len) const {
    len = memsize();
    char* ret = new char[len];
    memset(ret, 0, len);
    char* p = ret;
    SERIALIZE(p, _width);
    SERIALIZE(p, _height);
    SERIALIZE(p, _dotsPerMM);

    size_t nshapes = _shapes.size();
    SERIALIZE(p, nshapes);
    for (size_t i = 0; i < _shapes.size(); i++) {
        p = _shapes[i]->serialize(p);
    }

    return ret;
}


/**
 * @brief Deserialize the MapSymbol object from a byte array.
 * This function populates the MapSymbol object with properties and shapes from the provided byte array.
 * @param data The byte array containing the serialized MapSymbol data.
 * @return True if the deserialization was successful, false otherwise.
 * If the data is invalid or missing required fields, an error message is set. Use getErrorMessage() to retrieve it.
 */
bool MapSymbol::deserialize(char* data) {
    char* p = data;
    DESERIALIZE(p, _width);
    DESERIALIZE(p, _height);
    DESERIALIZE(p, _dotsPerMM);

    size_t nshapes;
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
        case SymShape::SYM_SYSTEM_FILL:
            shp = new SymSystemFill();
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


/**
 * @brief Extract the shapes from the MapSymbol object into a vector of MapSymbol pointers.
 * This function creates new MapSymbol objects for each shape in the current MapSymbol and returns them in a vector.
 * Each new MapSymbol will have the same width, height, and dots per mm as the original.
 * @return A vector of pointers to newly created MapSymbol objects.
 * The caller is responsible for deleting the returned objects.
 */
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



/**
 * @brief Set the dots per millimeter for the MapSymbol.
 * This function sets the dots per millimeter value, which is used for scaling the symbol when rendering.
 * @param dotsPerMM The number of dots per millimeter.
 */
void MapSymbol::setDotsPerMM(double dotsPerMM) {
    _dotsPerMM = dotsPerMM;
}


/**
 * @brief Get the shape at the specified index.
 * This function retrieves a pointer to the shape at the given index in the shapes vector.
 * @param idx The index of the shape to retrieve.
 * @return A pointer to the shape at the specified index, or nullptr if the index is out of bounds.
 */
const SymShape* MapSymbol::getShape(size_t idx) const {
    if (idx < _shapes.size()) {
        return _shapes[idx];
    }
    return nullptr;
}


/**
 * @brief Get the number of shapes in the MapSymbol.
 * This function returns the count of shapes stored in the MapSymbol object.
 * @return The number of shapes in the MapSymbol.
 */
size_t MapSymbol::getShapeCount() const {
    return _shapes.size();
}

/**
 * @brief Get the width of the MapSymbol.
 * This function returns the width of the MapSymbol in millimeters.
 * @return The width of the MapSymbol.
 */
double MapSymbol::getWidth() const {
    return _width;
}


/**
 * @brief Get the height of the MapSymbol.
 * This function returns the height of the MapSymbol in millimeters.
 * @return The height of the MapSymbol.
 */
double MapSymbol::getHeight() const {
    return _height;
}


/**
 * @brief Get the dots per millimeter of the MapSymbol.
 * This function returns the dots per millimeter value, which is used for scaling the symbol when rendering.
 * @return The dots per millimeter of the MapSymbol.
 */
double MapSymbol::getDotsPerMM() const {
    return _dotsPerMM;
}


/**
 * @brief Set the width of the MapSymbol.
 * This function sets the width of the MapSymbol in millimeters.
 * @param width The width to set for the MapSymbol.
 */
void MapSymbol::setWidth(double width) {
    _width = width;
}


/**
 * @brief Set the height of the MapSymbol.
 * This function sets the height of the MapSymbol in millimeters.
 * @param height The height to set for the MapSymbol.
 */
void MapSymbol::setHeight(double height) {
    _height = height;
}


/**
 * @brief Append a shape to the MapSymbol.
 * This function adds a new shape to the MapSymbol's shapes vector.
 * @param shape A pointer to the shape to append. If the pointer is null, it is ignored.
 * The shape is not cloned, so the caller must ensure the shape remains valid for the lifetime of the MapSymbol.
 * If the shape is null, it will not be added to the shapes vector.
 * If the symbol is destroyed, the shape will be deleted also.
 */
void MapSymbol::appendShape(SymShape* shape)
{
    if (shape) {
        _shapes.push_back(shape);
    }
}


