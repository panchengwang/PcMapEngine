#include <iostream>
#include <stdlib.h>
#include <MapCanvas.h>
#include <sys/time.h>
#include <geos.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/io/WKTReader.h>
#include <geos/io/WKTWriter.h>

int main(int argc, char** argv) {

    MapCanvas canvas;

    geos::geom::GeometryFactory::Ptr geomFactory = geos::geom::GeometryFactory::create();
    geos::io::WKTReader _wktReader(geomFactory.get());

    auto geo = _wktReader.read("POINT(45 45)");


    canvas.begin();
    for (int i = 0;i < 1;++i) {
        canvas.draw(geo.get());
    }

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
