#ifndef __MAP_CANVAS_H
#define __MAP_CANVAS_H

#include <map_engine_dll_exports.h>
#include <geos.h>
#include <iostream>
#include <string>
#include <opencv4/opencv2/opencv.hpp>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/util/GeometryEditor.h>
#include "AffineTransformOperation.h"

#include "../symbol/MapSymbol.h"
#include "../symbol/SymShape.h"
#include "../symbol/SymSystemLine.h"
#include <cairo.h>


class DLL_EXPORT MapCanvas {
public:
    MapCanvas();
    MapCanvas(const geos::geom::Envelope& env, double width, double height);
    ~MapCanvas();

    void setCanvasSize(double width, double height);
    void setEnvelope(double minX, double maxX, double minY, double maxY);
    void setEnvelope(const geos::geom::Envelope& env);
    void setDotsPerMM(double dotsPerMM);
    void setCenter(double x, double y);
    void setScale(double scale);
    void setFormat(const std::string& format);

    void setDefaultPointSymbol(const MapSymbol& symbol);
    void setDefaultLineSymbol(const MapSymbol& symbol);
    void setDefaultFillSymbol(const MapSymbol& symbol);

    void setStrokeStyle(SymShape* shp);

    void draw(const geos::geom::Geometry* geom);
    void draw(const geos::geom::Point* geom);

    void draw(const geos::geom::LineString* geom);
    void draw(const geos::geom::LineString* geom, SymSystemLine* symshp);

    void draw(const geos::geom::Polygon* geom);
    void draw(const geos::geom::GeometryCollection* geom);

    void draw(const std::string& wkt);

    bool begin();
    bool end();

    char* imageData(size_t& size);
    void log();

protected:
    void recalculateMapParameters();
    std::string createUUID();
    geos::geom::Geometry::Ptr mapToCanvas(const geos::geom::Geometry* geom);

    void initDefaultSymbols();
private:

    double _minX, _maxX, _minY, _maxY;
    double _width, _height;
    double _dotsPerMM;
    double _centerX, _centerY;
    double _scale;
    cairo_surface_t* _surface;
    cairo_t* _cairo;
    std::string _format;
    std::string _filename;

    cv::Mat _transformMatrix;

    AffineTransformOperation _affineOperator;
    geos::geom::GeometryFactory::Ptr _geomFactory;
    geos::geom::util::GeometryEditor _geomEditor;
    geos::io::WKTReader _wktReader;

    MapSymbol  _defaultPointSymbol;
    MapSymbol  _defaultLineSymbol;
    MapSymbol  _defaultFillSymbol;
};



#endif
