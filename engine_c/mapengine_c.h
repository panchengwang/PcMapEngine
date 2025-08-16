#ifndef __MAPENGINE_C_H__
#define __MAPENGINE_C_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef void* MAPENGINE_H;
    typedef void* MAPSYMBOL_H;



    MAPSYMBOL_H sym_create();
    void sym_destroy(MAPSYMBOL_H symbol);
    void sym_destroy_array(MAPSYMBOL_H* symbols, size_t len);

    MAPSYMBOL_H sym_from_json(MAPSYMBOL_H symbol, const char* json_str, uint32_t* ok);
    MAPSYMBOL_H sym_from_json_file(MAPSYMBOL_H symbol, const char* json_filename, uint32_t* ok);
    char* sym_to_json(MAPSYMBOL_H symbol, int32_t* len);
    const char* sym_get_error_message(MAPSYMBOL_H symbol);

    MAPSYMBOL_H sym_set_dotspermm(MAPENGINE_H symbol, double dotsPerMM);

    char* sym_serialize(MAPSYMBOL_H symbol, size_t* len);
    MAPSYMBOL_H sym_deserialize(const char* data);

    char* sym_to_image(MAPSYMBOL_H symbol, size_t* len);

    MAPSYMBOL_H* sym_extract(MAPSYMBOL_H symbol, size_t* len);
    MAPSYMBOL_H sym_get_shape_as_symbol(MAPSYMBOL_H symbol, size_t idx);


#ifdef __cplusplus
}
#endif

#endif
