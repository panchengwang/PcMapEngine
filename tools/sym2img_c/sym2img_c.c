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
    free(image);
    sym_destroy(symbol);

    // MAPSYMBOL_H* syms = sym_extract(sym2, &len);
    // for (size_t i = 0; i < len; ++i) {
    //     size_t datasize;
    //     image = sym_to_image(syms[i], &datasize);
    //     char filename[256];
    //     sprintf(filename, "%s_%d.png", argv[1], i);
    //     FILE* fd = fopen(filename, "wb");
    //     fwrite(image, datasize, 1, fd);
    //     fclose(fd);
    //     free(image);
    // }
    // for (size_t i = 0; i < len; ++i) {
    //     sym_destroy(syms[i]);
    // }
    // free(syms);


    sym_destroy(sym2);


    return EXIT_SUCCESS;
}
