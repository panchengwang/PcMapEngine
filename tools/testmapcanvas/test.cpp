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

    canvas.setDotsPerMM(300 / 25.4);
    canvas.setCanvasSize(200, 160);
    MapSymbol defaultPointSymbol;
    if (!defaultPointSymbol.fromJson(R"({
        "width": 10.0,
        "height":10.0,
        "shapes":[{
            "type":"circle",
            "stroke":{
                "color":[255, 0, 0, 255],
                "width":0.5,
                "cap":"round",
                "join":"miter",
                "dashes":[1,0.5]
            },
            "fill":{
                "type":"solid",
                "color":[128, 0, 0, 255]
            },
            "center":[0.0, 0.0],
            "radius":0.80
        }]
    })")) {
        std::cerr << defaultPointSymbol.getErrorMessage() << std::endl;
    }
    else {
        canvas.setDefaultPointSymbol(defaultPointSymbol);
    }


    geos::geom::GeometryFactory::Ptr geomFactory = geos::geom::GeometryFactory::create();
    geos::io::WKTReader _wktReader(geomFactory.get());




    canvas.begin();

    auto geo = _wktReader.read("POINT(0 0)");

    canvas.draw(geo.get());


    geo = _wktReader.read("POINT(112 28)");
    canvas.draw(geo.get());

    geo = _wktReader.read("POINT(0 40)");
    canvas.draw(geo.get());

    geo = _wktReader.read("POINT(-112 28)");
    canvas.draw(geo.get());

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
