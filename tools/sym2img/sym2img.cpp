#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <filesystem>
#include "../../engine/symbol/MapSymbol.h"

// std::string readFile(const std::string& filePath) {
//     if (!std::filesystem::exists(filePath)) {
//         throw std::runtime_error("file not exists: " + filePath);
//     }

//     std::ifstream file(filePath, std::ios::binary);
//     if (!file.is_open()) {
//         throw std::runtime_error("can not open file: " + filePath);
//     }

//     auto fileSize = std::filesystem::file_size(filePath);
//     std::string content(fileSize + 1, '\0');

//     file.read(content.data(), fileSize);
//     file.close();
//     return content;
// }

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cerr << "usage:" << std::endl
            << "    " << argv[0] << " <symbol file name>" << std::endl
            << "    example: " << argv[0] << " circle.json  " << std::endl;
    }
    std::string filename = argv[1];
    // std::string content = readFile(filename);
    MapSymbol symbol;
    if (!symbol.fromJsonFile(filename.c_str())) {
        std::cerr << symbol.getErrorMessage() << std::endl;
    }
    // std::cerr << symbol.toJson() << std::endl;

    // size_t memsize = symbol.memsize();
    std::cerr << "memsize: " << symbol.memsize() << std::endl;

    size_t len;
    char* buf = symbol.serialize(len);
    std::cerr << "ok..." << std::endl;
    MapSymbol sym2;
    if (!sym2.deserialize(buf)) {
        std::cerr << sym2.getErrorMessage() << std::endl;
        return -1;
    }
    std::cerr << "ok..." << std::endl;
    char* data = sym2.imageData(len);
    fwrite(data, len, 1, stdout);

    free(buf);
    free(data);

    return EXIT_SUCCESS;
}
