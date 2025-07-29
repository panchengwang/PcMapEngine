#include "mapengine_c.h"
#include "../engine/symbol/MapSymbol.h"
#include <string.h>

MAPSYMBOL_H sym_create() {
    MAPSYMBOL_H symh = nullptr;
    symh = new MapSymbol();
    return symh;
}

void sym_destroy(MAPSYMBOL_H symbol) {
    MapSymbol* sym = (MapSymbol*)symbol;
    delete sym;
}

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

char* sym_to_json(MAPSYMBOL_H symbol, int32_t* len) {
    MapSymbol* sym = (MapSymbol*)symbol;
    std::string json_str = sym->toJson();
    char* cstr = (char*)malloc(json_str.size() + 1);
    memcpy(cstr, json_str.c_str(), json_str.size());
    cstr[json_str.size()] = '\0';
    return cstr;
}

char* sym_serialize(MAPSYMBOL_H symbol, size_t* len) {
    MapSymbol* sym = (MapSymbol*)symbol;
    char* data = sym->serialize(*len);
    char* buf = (char*)malloc(*len);
    memcpy(buf, data, *len);
    delete[] data;
    return buf;
}

MAPSYMBOL_H sym_deserialize(const char* data) {
    MAPSYMBOL_H symbol = sym_create();
    MapSymbol* sym = (MapSymbol*)symbol;
    sym->deserialize((char*)data);
    return symbol;
}

char* sym_to_image(MAPSYMBOL_H symbol, size_t* len) {
    MapSymbol* sym = (MapSymbol*)symbol;
    return sym->imageData(*len);
}

const char* sym_get_error_message(MAPSYMBOL_H symbol) {
    MapSymbol* sym = (MapSymbol*)symbol;
    return sym->getErrorMessage().c_str();
}