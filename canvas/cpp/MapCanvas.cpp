#include <MapCanvas.h>
#include <iostream>

MapCanvas::MapCanvas() {
    _dotsPerMM = 144.0 / 25.4;
    _width = 800 / _dotsPerMM;
    _height = 600 / _dotsPerMM;
    _minX = -180;
    _maxX = 180;
    _minY = -90;
    _maxY = 90;
    _centerX = 0;
    _centerY = 0;
    recalculateMapParameters();
}

MapCanvas::~MapCanvas() {

}

void MapCanvas::recalculateMapParameters() {
    double sx = (_maxX - _minX) / _width;
    double sy = (_maxY - _minY) / _height;
    _scale = (sx < sy) ? sx : sy;

    _minX = _centerX - (_width * 0.5) * _scale;
    _maxX = _centerX + (_width * 0.5) * _scale;
    _minY = _centerY - (_height * 0.5) * _scale;
    _maxY = _centerY + (_height * 0.5) * _scale;
}



MapCanvas::MapCanvas(const geos::geom::Envelope& env, double width, double height) {
    _dotsPerMM = 144.0 / 25.4;
    _width = width;
    _height = height;
    _minX = env.getMinX();
    _maxX = env.getMaxX();
    _minY = env.getMinY();
    _maxY = env.getMaxY();
    _centerX = (_minX + _maxX) * 0.5;
    _centerY = (_minY + _maxY) * 0.5;
    recalculateMapParameters();
}



void MapCanvas::setCanvasSize(double width, double height) {
    _width = width;
    _height = height;
}


void MapCanvas::setEnvelope(double minX, double maxX, double minY, double maxY) {
    _minX = minX;
    _maxX = maxX;
    _minY = minY;
    _maxY = maxY;
    _centerX = (_minX + _maxX) * 0.5;
    _centerY = (_minY + _maxY) * 0.5;
}

void MapCanvas::setEnvelope(const geos::geom::Envelope& env) {
    setEnvelope(env.getMinX(), env.getMaxX(), env.getMinY(), env.getMaxY());
}

void MapCanvas::setDotsPerMM(double dotsPerMM) {
    _dotsPerMM = dotsPerMM;
}

void MapCanvas::setCenter(double x, double y) {
    _centerX = x;
    _centerY = y;
    _minX = _centerX - (_width * 0.5) * _scale;
    _maxX = _centerX + (_width * 0.5) * _scale;
    _minY = _centerY - (_height * 0.5) * _scale;
    _maxY = _centerY + (_height * 0.5) * _scale;
}

void MapCanvas::setScale(double scale) {
    _scale = scale;

}


void MapCanvas::begin() {
    recalculateMapParameters();
}


void MapCanvas::end() {

}



void MapCanvas::log() {
    std::cout << "MapCanvas:    minx: " << _minX << ", maxx: " << _maxX << ", miny: " << _minY << ", maxy: " << _maxY << std::endl;
    std::cout << "MapCanvas:    centerx: " << _centerX << ", centery: " << _centerY << std::endl;
    std::cout << "MapCanvas:    width: " << _width << ", height: " << _height << std::endl;
    std::cout << "MapCanvas:    scale: " << _scale << std::endl;
}