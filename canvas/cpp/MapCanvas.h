#ifndef __MAP_CANVAS_H
#define __MAP_CANVAS_H

#include <map_engine_dll_exports.h>
#include <geos.h>
#include <iostream>

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

    void begin();
    void end();

    void log();

protected:
    void recalculateMapParameters();

private:

    double _minX, _maxX, _minY, _maxY;
    double _width, _height;
    double _dotsPerMM;
    double _centerX, _centerY;
    double _scale;

};



#endif
