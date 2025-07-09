#include <iostream>
#include <stdlib.h>
#include <MapCanvas.h>
#include <sys/time.h>
#include <geos.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/io/WKTReader.h>
#include <geos/io/WKTWriter.h>

#include <fstream>
#include <string>
#include <filesystem>

std::string readFile(const std::string& filePath) {
    if (!std::filesystem::exists(filePath)) {
        throw std::runtime_error("file not exists: " + filePath);
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("can not open file: " + filePath);
    }

    auto fileSize = std::filesystem::file_size(filePath);
    std::string content(fileSize, '\0');

    file.read(content.data(), fileSize);
    file.close();
    return content;
}

int main(int argc, char** argv) {

    std::cerr << "usage:" << std::endl
        << "    " << argv[0] << " <default_point_symbol>" << std::endl
        << "    example: " << argv[0] << " /mnt/e/PcMapEngine/tools/testmapcanvas/default_point_sym.json  > a.png" << std::endl;
    MapCanvas canvas;

    canvas.setDotsPerMM(600 / 25.4);
    canvas.setCanvasSize(200, 160);
    MapSymbol defaultPointSymbol;
    // if (!defaultPointSymbol.fromJson(readFile(std::filesystem::canonical(argv[0]).parent_path().c_str() + std::string("/default_point_sym.json")))) {
    if (!defaultPointSymbol.fromJson(readFile(argv[1]))) {
        std::cerr << defaultPointSymbol.getErrorMessage() << std::endl;
        return 1;
    }
    else {
        canvas.setDefaultPointSymbol(defaultPointSymbol);
    }


    geos::geom::GeometryFactory::Ptr geomFactory = geos::geom::GeometryFactory::create();
    geos::io::WKTReader _wktReader(geomFactory.get());




    canvas.begin();

    auto geo = _wktReader.read("POINT(0 0)");

    canvas.draw(geo.get());


    // geo = _wktReader.read("POINT(112 28)");
    // canvas.draw(geo.get());

    // geo = _wktReader.read("POINT(0 40)");
    // canvas.draw(geo.get());

    // geo = _wktReader.read("POINT(-112 28)");
    // canvas.draw(geo.get());

    // canvas.draw("LINESTRING(1 2,3 4)");

    canvas.log();
    canvas.end();

    size_t len = 0;
    char* data = canvas.data(len);

    for (size_t i = 0;i < len;++i) {
        std::cout << data[i];
    }

    delete[] data;
    return EXIT_SUCCESS;
}
