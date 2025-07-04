#include "SymFill.h"


json_object* SymFill::toJson() const {
    json_object* json = json_object_new_object();
    if (_type == FILL_SOLID) {
        json_object_object_add(json, "type", json_object_new_string("solid"));
    }
    else if (_type == FILL_GRADIENT) {
        json_object_object_add(json, "type", json_object_new_string("gradient"));
    }
    else if (_type == FILL_IMAGE) {
        json_object_object_add(json, "type", json_object_new_string("image"));
    }
    return json;
}