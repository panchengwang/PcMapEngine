#ifndef _JSON_UTILS_H__
#define _JSON_UTILS_H__

#include <json-c/json.h> 

#define JSON_GET_BOOLEAN(obj, name, val, errMsg)                                                                        \
    {                                                                                                                   \
        json_object* myobj;                                                                                             \
        if (!json_object_object_get_ex(obj, name, &myobj)) {                                                            \
            errMsg = std::string("Missing  field: ") + name;                                                            \
            return false;                                                                                               \
        }                                                                                                               \
        if (json_object_get_type(myobj) != json_type_boolean) {                                                         \
            errMsg = std::string("Invalid type for field: ") + name + " (boolena)";                                     \
            return false;                                                                                               \
        }                                                                                                               \
        val = json_object_get_boolean(myobj);                                                                           \
    }


#define JSON_GET_DOUBLE(obj, name, val, errMsg)                                                                         \
    {                                                                                                                   \
        json_object* myobj;                                                                                             \
        if (!json_object_object_get_ex(obj, name, &myobj)) {                                                            \
            errMsg = std::string("Missing  field: ") + name;                                                            \
            return false;                                                                                               \
        }                                                                                                               \
        if (json_object_get_type(myobj) != json_type_double && json_object_get_type(myobj) != json_type_int) {          \
            errMsg = std::string("Invalid type for field: ") + name + " (double)";                                      \
            return false;                                                                                               \
        }                                                                                                               \
        val = json_object_get_double(myobj);                                                                            \
    }

#define JSON_GET_INT(obj, name, val, errMsg)                                                                            \
    {                                                                                                                   \
        json_object* myobj;                                                                                             \
        if (!json_object_object_get_ex(obj, name, &myobj)) {                                                            \
            errMsg = std::string("Missing  field: ") + name;                                                            \
            return false;                                                                                               \
        }                                                                                                               \
        if (json_object_get_type(myobj) != json_type_int) {                                                             \
            errMsg = std::string("Invalid type for field: ") + name + " (int)";                                         \
            return false;                                                                                               \
        }                                                                                                               \
        val = json_object_get_int(myobj);                                                                               \
    }

#define JSON_GET_STRING(obj, name, val, errMsg)                                                                         \
    {                                                                                                                   \
        json_object* myobj;                                                                                             \
        if (!json_object_object_get_ex(obj, name, &myobj)) {                                                            \
            errMsg = std::string("Missing  field: ") + name;                                                            \
            return false;                                                                                               \
        }                                                                                                               \
        if (json_object_get_type(myobj) != json_type_string) {                                                          \
            errMsg = std::string("Invalid type for field: ") + name + " (string)";                                      \
            return false;                                                                                               \
        }                                                                                                               \
        val = json_object_get_string(myobj);                                                                            \
    }

#define JSON_GET_OBJECT(obj, name, val, errMsg)                                                                         \
    {                                                                                                                   \
        json_object* myobj;                                                                                             \
        if (!json_object_object_get_ex(obj, name, &myobj)) {                                                            \
            errMsg = std::string("Missing  field: ") + name;                                                            \
            return false;                                                                                               \
        }                                                                                                               \
        if (json_object_get_type(myobj) != json_type_object) {                                                          \
            errMsg = std::string("Invalid type for field: ") + name + " (object)";                                      \
            return false;                                                                                               \
        }                                                                                                               \
        val = myobj;                                                                                                    \
    }



#define JSON_GET_ARRAY(obj, name, val, errMsg)                                                                      \
    {                                                                                                                   \
        json_object* myobj;                                                                                             \
        if (!json_object_object_get_ex(obj, name, &myobj)) {                                                            \
            errMsg = std::string("Missing  field: ") + name;                                                            \
            return false;                                                                                               \
        }                                                                                                               \
        if (json_object_get_type(myobj) != json_type_array) {                                                           \
            errMsg = std::string("Invalid type for field: ") + name + " (array)";                                       \
            return false;                                                                                               \
        }                                                                                                               \
        val = myobj;                                                                                                    \
    }



#define JSON_GET_ARRAY_OBJECT(obj, name, val, errMsg)                                                                   \
    {                                                                                                                   \
        json_object* myobj;                                                                                             \
        if (!json_object_object_get_ex(obj, name, &myobj)) {                                                            \
            errMsg = std::string("Missing  field: ") + name;                                                            \
            return false;                                                                                               \
        }                                                                                                               \
        if (json_object_get_type(myobj) != json_type_array && json_object_get_type(myobj) !=json_type_object) {         \
            errMsg = std::string("Invalid type for field: ") + name + " (array)";                                       \
            return false;                                                                                               \
        }                                                                                                               \
        val = myobj;                                                                                                    \
    }



#define JSON_GET_ARRAY_OBJECT_STRING(obj, name, val, errMsg)                                                            \
    {                                                                                                                   \
        json_object* myobj;                                                                                             \
        if (!json_object_object_get_ex(obj, name, &myobj)) {                                                            \
            errMsg = std::string("Missing  field: ") + name;                                                            \
            return false;                                                                                               \
        }                                                                                                               \
        json_type mytype = json_object_get_type(myobj);                                                                 \
        if (mytype != json_type_array && mytype != json_type_string && mytype != json_type_object ) {                    \
            errMsg = std::string("Invalid type for field: ") + name + " (array,object,string)";                         \
            return false;                                                                                               \
        }                                                                                                               \
        val = myobj;                                                                                                    \
    }


#endif
