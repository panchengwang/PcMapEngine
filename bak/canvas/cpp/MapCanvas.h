#ifndef __MAP_CANVAS_H
#define __MAP_CANVAS_H

#include <map_engine_dll_exports.h>
#include <geos.h>
#include <iostream>
#include <string>
// #include <SkSurface.h>
// #include <SkCanvas.h>
#include <opencv4/opencv2/opencv.hpp>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/util/GeometryEditor.h>
#include "AffineTransformOperation.h"
#include "../../symbol/cpp/MapSymbol.h"
#include <cairo.h>

/**
 * @class MapCanvas
 * @brief MapCanvas 类用于绘制地图的画布。
 *
 * MapCanvas 类提供一个用于在图形界面上显示地图的画布。MapCanvas在绘制是使用的长度单位为毫米(mm)。
 */
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


    void draw(const geos::geom::Geometry* geom);
    void draw(const geos::geom::Point* geom);
    void draw(const geos::geom::LineString* geom);
    void draw(const geos::geom::Polygon* geom);
    void draw(const geos::geom::GeometryCollection* geom);

    void draw(const std::string& wkt);

    bool begin();
    bool end();

    char* data(size_t& len);
    void log();

protected:
    void recalculateMapParameters();
    std::string createUUID();

    geos::geom::Geometry::Ptr mapToCanvas(const geos::geom::Geometry* geom);

private:

    double _minX, _maxX, _minY, _maxY;
    double _width, _height;
    double _dotsPerMM;
    double _centerX, _centerY;
    double _scale;
    cairo_surface_t* _surface;
    cairo_t* _canvas;
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
