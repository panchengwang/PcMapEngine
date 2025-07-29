#include <stdio.h>
#include <stdlib.h>
#include "../engine_c/mapengine_c.h"

int main(int argc, char** argv) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <symbol file ,json>\n", argv[0]);
        return EXIT_FAILURE;
    }

    MAPSYMBOL_H symbol = sym_create();
    int ok = 1;
    symbol = sym_from_json_file(symbol, argv[1], &ok);
    if (!ok) {
        fprintf(stderr, "Error: %s\n", sym_get_error_message(symbol));
        sym_destroy(symbol);
        return EXIT_FAILURE;
    }

    size_t len = 0;
    char* data = sym_serialize(symbol, &len);
    MAPSYMBOL_H sym2 = sym_deserialize(data);
    free(data);

    char* image = sym_to_image(sym2, &len);
    fwrite(image, len, 1, stdout);
    // for(size_t i = 0; i < len; ++i) {
    //     fwrite(&image[i], 1, sizeof(char), stdout);
    // }
    free(image);
    sym_destroy(symbol);
    sym_destroy(sym2);


    return EXIT_SUCCESS;
}
