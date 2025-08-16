#include "mapengine_c.h"
#include "../engine/symbol/MapSymbol.h"
#include <string.h>


/**
 * @brief Creates a new MapSymbol object.
 * @return A pointer to the newly created MapSymbol object.
 *   To free the memory allocated by this function, use:
 *      sym_destroy(symbol);
 */
MAPSYMBOL_H sym_create() {
    MAPSYMBOL_H symh = nullptr;
    symh = new MapSymbol();
    return symh;
}


/**
 * @brief Destroys a MapSymbol object.
 * @param symbol The MapSymbol object to destroy.
 */
void sym_destroy(MAPSYMBOL_H symbol) {
    MapSymbol* sym = (MapSymbol*)symbol;
    delete sym;
}

/**
 * @brief Destroys an array of MapSymbol objects.
 * @param symbols An array of MapSymbol objects.
 * @param len The length of the array.
 */
void sym_destroy_array(MAPSYMBOL_H* symbols, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        sym_destroy(symbols[i]);
    }
    free(symbols);
}

/**
 * @brief Loads a MapSymbol object from a JSON string.
 * @param symbol The MapSymbol object to load.
 * @param json_str Json string of a map symbol.
 * @param ok A pointer to a uint32_t variable that will be set to 1 if the operation is successful, or 0 otherwise.
 * @return The MapSymbol object.
 *   To free the memory allocated by this function, use:
 *      sym_destroy(symbol);
 */
MAPSYMBOL_H sym_from_json(MAPSYMBOL_H symbol, const char* json_str, uint32_t* ok) {
    MapSymbol* sym = (MapSymbol*)symbol;
    if (sym->fromJson(json_str)) {
        *ok = 1;
    }
    else {
        *ok = 0;
    }
    return symbol;
}

/**
 * @brief Loads a MapSymbol object from a JSON file.
 * @param symbol The MapSymbol object to load.
 * @param json_filename The path to the JSON file.
 * @param ok A pointer to a uint32_t variable that will be set to 1 if the operation is successful, or 0 otherwise.
 * @return The MapSymbol object.
 *   To free the memory allocated by this function, use:
 *      sym_destroy(symbol);
 */
MAPSYMBOL_H sym_from_json_file(MAPSYMBOL_H symbol, const char* json_filename, uint32_t* ok) {
    MapSymbol* sym = (MapSymbol*)symbol;
    if (sym->fromJsonFile(json_filename)) {
        *ok = 1;
    }
    else {
        *ok = 0;
    }
    return symbol;
}

/**
 * @brief Converts a MapSymbol object to JSON string.
 * @param symbol The MapSymbol object to convert.
 * @param len The length of the JSON string.
 * @return A pointer to the JSON string.
 *        The caller is responsible for releasing the memory allocated by this function.
 *        To free the memory allocated by this function, use:
 *      free(json_str);
 */
char* sym_to_json(MAPSYMBOL_H symbol, int32_t* len) {
    MapSymbol* sym = (MapSymbol*)symbol;
    std::string json_str = sym->toJson();
    *len = json_str.size();
    char* cstr = (char*)malloc(*len + 1);
    memcpy(cstr, json_str.c_str(), *len);
    cstr[*len] = '\0';
    return cstr;
}


/**
 * @brief Serializes a MapSymbol object to binary data.
 * @param symbol The MapSymbol object to serialize.
 *      The caller is responsible for releasing the memory allocated by this function.
 *      To free the memory allocated by this function, use:
 *      free(data);
 */
char* sym_serialize(MAPSYMBOL_H symbol, size_t* len) {
    MapSymbol* sym = (MapSymbol*)symbol;
    char* data = sym->serialize(*len);
    char* buf = (char*)malloc(*len);
    memcpy(buf, data, *len);
    delete[] data;
    return buf;
}

/**
 * @brief Deserializes a MapSymbol object from serialized data.
 * @param data The serialized MapSymbol object.
 */
MAPSYMBOL_H sym_deserialize(const char* data) {
    MAPSYMBOL_H symbol = sym_create();
    MapSymbol* sym = (MapSymbol*)symbol;
    sym->deserialize((char*)data);
    return symbol;
}


/**
 * @brief Converts a MapSymbol object to an image data array.
 * @param symbol The MapSymbol object to convert.
 * @return char* A pointer to the image data array.
 *      The caller is responsible for releasing the memory allocated by this function.
 */
char* sym_to_image(MAPSYMBOL_H symbol, size_t* len) {
    MapSymbol* sym = (MapSymbol*)symbol;
    return sym->imageData(*len);
}

/**
 * @brief Get the error message from a MapSymbol object.
 * @param symbol The MapSymbol object to get the error message from.
 */
const char* sym_get_error_message(MAPSYMBOL_H symbol) {
    MapSymbol* sym = (MapSymbol*)symbol;
    return sym->getErrorMessage().c_str();
}


/**
 * @brief Extracts the symbol data from a MapSymbol object.
 *      Free the memory returned by this way：
 *
 *      size_t len;
 *      MAPSYMBOL_H* symarr = sym_extract(symbol, &len);
 *
 *      for(size_t i = 0; i < len; ++i) {
 *          sym_destroy(symarr[i]);
 *      }
 *      free(symarr);
 *
 *   or
 *
 *      sym_destroy_array(symarr, len);
 * @param symbol The MapSymbol object to extract data from.
 * @param len The length of the extracted symbol array.
 *
 */
MAPSYMBOL_H* sym_extract(MAPSYMBOL_H symbol, size_t* len) {
    std::vector<MapSymbol*> syms = ((MapSymbol*)symbol)->extract();
    *len = syms.size();
    MAPSYMBOL_H* symarr = (MAPSYMBOL_H*)malloc(sizeof(MapSymbol*) * syms.size());
    for (size_t i = 0; i < syms.size(); ++i) {
        symarr[i] = (MAPSYMBOL_H)syms[i];
    }
    return symarr;
}

/**
 * @brief Gets a specific shape from a MapSymbol object as a new MapSymbol object.
 * @param symbol The MapSymbol object to get the shape from.
 * @param idx The index of the shape to get.
 * @return A new MapSymbol object containing the specified shape.
 *     The caller is responsible for releasing the memory allocated by this function.
 *     To free the memory allocated by this function, use:
 *          sym_destroy;
 */
MAPSYMBOL_H sym_get_shape_as_symbol(MAPSYMBOL_H symbol, size_t idx) {
    MapSymbol* sym = (MapSymbol*)symbol;
    if (idx >= sym->getShapeCount()) {
        return nullptr;
    }

    const SymShape* shape = sym->getShape(idx);

    MapSymbol* ret = new MapSymbol();
    ret->setWidth(sym->getWidth());
    ret->setHeight(sym->getHeight());
    ret->setDotsPerMM(sym->getDotsPerMM());
    ret->appendShape(shape->clone());
    return (MAPSYMBOL_H)ret;
}

/**
 * @brief Sets the dots per millimeter for a MapSymbol object.
 * @param symbol The MapSymbol object to set the dots per millimeter for.
 * @param dotsPerMM The dots per millimeter value to set.
 * @return The MapSymbol handler with the updated dots per millimeter.
 */
MAPSYMBOL_H sym_set_dotspermm(MAPENGINE_H symbol, double dotsPerMM) {
    MapSymbol* sym = (MapSymbol*)symbol;
    sym->setDotsPerMM(dotsPerMM);
    return symbol;
}