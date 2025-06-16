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

    GeometryFactory::Ptr factory = GeometryFactory::create();

    /*
    * Reader requires a factory to bind the geometry to
    * for shared resources like the PrecisionModel
    */
    geos::io::WKTReader reader(*factory);


    canvas.begin();
    canvas.draw(reader.read("POINT(0 0)").get());
    canvas.draw(reader.read("LINESTRING(0 0, 10 10)").get());
    canvas.draw(reader.read("POLYGON((5 5, 15 5, 15 15, 5 15, 5 5))").get());

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
