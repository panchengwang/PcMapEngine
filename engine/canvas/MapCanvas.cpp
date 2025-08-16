#include <MapCanvas.h>
#include <iostream>
#include <algorithm>
// #include <SkData.h>
// #include <SkImage.h>
// #include <SkStream.h>
// #include <SkSurface.h>
// #include <SkPath.h>
// #include <SkPngEncoder.h>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <filesystem>
#include <opencv4/opencv2/opencv.hpp>
#include <geos/geom/util/GeometryEditor.h>
#include "AffineTransformer.h"

MapCanvas::MapCanvas() {
    _dotsPerMM = 96.0 / 25.4;
    _width = 800 / _dotsPerMM;
    _height = 600 / _dotsPerMM;
    _minX = -180;
    _maxX = 180;
    _minY = -90;
    _maxY = 90;
    _centerX = 0;
    _centerY = 0;
    recalculateMapParameters();
    _surface = NULL;
    _cairo = NULL;
    setFormat("PNG");

    _filename = createUUID();
    _geomFactory = geos::geom::GeometryFactory::create();
    _geomEditor = geos::geom::util::GeometryEditor(_geomFactory.get());
    _affineOperator.setGeometryFactory(_geomFactory.get());
    _wktReader = geos::io::WKTReader(_geomFactory.get());

    initDefaultSymbols();
}


MapCanvas::MapCanvas(const geos::geom::Envelope& env, double width, double height) {
    _dotsPerMM = 96.0 / 25.4;
    _width = width;
    _height = height;
    _minX = env.getMinX();
    _maxX = env.getMaxX();
    _minY = env.getMinY();
    _maxY = env.getMaxY();
    _centerX = (_minX + _maxX) * 0.5;
    _centerY = (_minY + _maxY) * 0.5;
    recalculateMapParameters();
    _surface = NULL;
    _cairo = NULL;

    setFormat("PNG");
    _filename = createUUID();
    _geomFactory = geos::geom::GeometryFactory::create();
    _geomEditor = geos::geom::util::GeometryEditor(_geomFactory.get());
    _affineOperator.setGeometryFactory(_geomFactory.get());
    _wktReader = geos::io::WKTReader(_geomFactory.get());

    initDefaultSymbols();
}


void MapCanvas::initDefaultSymbols() {
    _defaultPointSymbol.fromJson(R"({
        "width": 3.0,
        "height": 3.0,
        "dotspermm": 3.7795275590551185,
        "shapes": [
          {
            "type": "regularpolygon",
            "stroke": {
              "color": [0, 0, 0, 255],
              "width": 0.1,
              "cap": "round",
              "join": "round",
              "dashes": [1, 0]
            },
            "fill": {
              "type": "solid",
              "color": [255, 0, 255, 127]
            },
            "center": [0.0, 0.0],
            "radius": 0.9,
            "sides": 8,
            "rotation": 0
          }
        ]
    })");

    _defaultLineSymbol.fromJson(R"({
        "width": 100.0,
        "height": 2,
        "dotspermm": 3.7795275590551185,
        "shapes": [{
            "type": "systemline",
            "stroke": {
                "color": [0, 0, 0, 255],
                "width": 0.2,
                "cap": "butt",
                "join": "miter",
                "dashes": [2, 0]
            }
        }]
    })");


}

MapCanvas::~MapCanvas() {

}

void MapCanvas::recalculateMapParameters() {
    double sx = _width / (_maxX - _minX);
    double sy = _height / (_maxY - _minY);
    _scale = (sx < sy) ? sx : sy;

    _minX = _centerX - (_width * 0.5) / _scale;
    _maxX = _centerX + (_width * 0.5) / _scale;
    _minY = _centerY - (_height * 0.5) / _scale;
    _maxY = _centerY + (_height * 0.5) / _scale;


    // std::vector<cv::Point2f> srcPoints = {
    //     cv::Point2f(_minX, _maxY),    // 左上
    //     cv::Point2f(_minX, _minY),   // 左下
    //     cv::Point2f(_maxX, _minY)  // 右下
    // };

    // std::vector<cv::Point2f> dstPoints = {
    //     cv::Point2f(0, 0),   // 变换后左上
    //     cv::Point2f(0, _height * _dotsPerMM),  // 变换后左下
    //     cv::Point2f(_width * _dotsPerMM, _height * _dotsPerMM)  // 变换后右下
    // };
    // cv::Mat transformMatrix = cv::getAffineTransform(srcPoints, dstPoints);
    // _transformMatrix = transformMatrix;
    // _affineOperator.setMatirx(_transformMatrix);
    // _affineOperator.log();

    AffineTransformer _affineTransformer;
    _affineTransformer.from3ControlPoints(
        _minX, _maxY, _minX, _minY, _maxX, _minY,
        0, 0, 0, _height * _dotsPerMM, _width * _dotsPerMM, _height * _dotsPerMM
    );
    _affineOperator.setMatrix(
        _affineTransformer.a(), _affineTransformer.b(),
        _affineTransformer.c(), _affineTransformer.d(),
        _affineTransformer.e(), _affineTransformer.f()
    );
}



void MapCanvas::setFormat(const std::string& format) {
    _format = format;
    std::transform(_format.begin(), _format.end(), _format.begin(), ::tolower);
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


bool MapCanvas::begin() {
    recalculateMapParameters();

    if (_format == "png" || _format == "jpg" || _format == "jpeg") {
        _surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, _width * _dotsPerMM, _height * _dotsPerMM);
        _cairo = cairo_create(_surface);
    }
    else if (_format == "pdf") {

        return false;
    }
    else {
        return false;
    }


    if (_cairo != NULL) {
        cairo_set_operator(_cairo, CAIRO_OPERATOR_SOURCE);
        cairo_set_source_rgba(_cairo, 0, 0, 0, 0);
        cairo_paint(_cairo);
        cairo_set_operator(_cairo, CAIRO_OPERATOR_OVER);
        cairo_set_antialias(_cairo, CAIRO_ANTIALIAS_DEFAULT);
    }

    return true;
}



geos::geom::Geometry::Ptr MapCanvas::mapToCanvas(const geos::geom::Geometry* geom) {

}

bool MapCanvas::end() {
    if (_surface) {
        cairo_surface_flush(_surface);
    }
    return true;
}



void MapCanvas::log() {
    // std::cerr << "MapCanvas:    minx: " << _minX << ", maxx: " << _maxX << ", miny: " << _minY << ", maxy: " << _maxY << std::endl;
    // std::cerr << "MapCanvas:    centerx: " << _centerX << ", centery: " << _centerY << std::endl;
    // std::cerr << "MapCanvas:    width: " << _width << ", height: " << _height << std::endl;
    // std::cerr << "MapCanvas:    scale: " << _scale << std::endl;
    // std::cerr << "MapCanvas:    dotsPerMM: " << _dotsPerMM << std::endl;
    // std::cerr << "MapCanvas:    format: " << _format << std::endl;
    // std::cerr << "MapCanvas:    filename: " << _filename << std::endl;

    // std::cerr << "[ "
    //     << _transformMatrix.at<double>(0, 0) << ", "
    //     << _transformMatrix.at<double>(0, 1) << ", "
    //     << _transformMatrix.at<double>(0, 2) << " ]\n";

    // std::cerr << "[ "
    //     << _transformMatrix.at<double>(1, 0) << ", "
    //     << _transformMatrix.at<double>(1, 1) << ", "
    //     << _transformMatrix.at<double>(1, 2) << " ]"
    //     << std::endl;

    // std::cerr << _defaultPointSymbol.toJson() << std::endl;
}


std::string MapCanvas::createUUID() {
    // std::string myuuid("");
    // uuid_t* uuid;
    // uuid_rc_t rc;
    // char* uuid_str;
    // rc = uuid_create(&uuid);
    // if (rc != UUID_RC_OK) {
    //     return "";
    // }
    // rc = uuid_make(uuid, UUID_MAKE_V1);
    // if (rc != UUID_RC_OK) {
    //     uuid_destroy(uuid);
    //     return "";
    // }

    // rc = uuid_export(uuid, UUID_FMT_STR, &uuid_str, NULL);
    // if (rc == UUID_RC_OK) {
    //     myuuid = uuid_str;
    //     free(uuid_str);  // 释放内存
    // }
    // return myuuid;

    boost::uuids::random_generator generator;
    boost::uuids::uuid uuid = generator();
    return std::filesystem::temp_directory_path().string() + "/" + boost::uuids::to_string(uuid);
}


typedef struct {
    char* data;
    size_t size;
}PngBuffer;

static cairo_status_t
my_png_save(void* closure,
    const unsigned char* data,
    unsigned int length) {
    PngBuffer* buffer = (PngBuffer*)closure;

    // 重新分配内存以容纳新数据
    char* new_data = (char*)realloc(buffer->data, buffer->size + length);
    if (!new_data) {
        return CAIRO_STATUS_NO_MEMORY;
    }

    buffer->data = new_data;
    memcpy(buffer->data + buffer->size, data, length);
    buffer->size += length;

    return CAIRO_STATUS_SUCCESS;
}



char* MapCanvas::imageData(size_t& size) {

    // cairo_surface_set_device_scale(_surface, _width / 1000.0, _height / 1000.0);
    cairo_surface_set_fallback_resolution(_surface, _dotsPerMM, _dotsPerMM);

    PngBuffer buffer = { NULL, 0 };

    cairo_surface_write_to_png_stream(_surface, my_png_save, &buffer);
    size = buffer.size;
    // std::vector<char> png_data;

    // // 将表面写入内存
    // cairo_status_t status = cairo_surface_write_to_png_stream(
    //     _surface,
    //     [](void* closure, const unsigned char* data, unsigned int length) -> cairo_status_t {
    //         auto& vec = *static_cast<std::vector<char>*>(closure);
    //         vec.insert(vec.end(), data, data + length);
    //         return CAIRO_STATUS_SUCCESS;
    //     },
    //     &png_data);

    return buffer.data;
}

void MapCanvas::draw(const std::string& wkt) {
    auto geo = _wktReader.read(wkt.c_str());
    draw(geo.get());
}


void MapCanvas::draw(const geos::geom::Geometry* geom) {
    if (!geom) return;
    if (geom->isEmpty()) return;

    std::unique_ptr<geos::geom::Geometry> transformedGeometry(
        _geomEditor.edit(geom, &_affineOperator)
    );

    // 绘制几何对象
    switch (transformedGeometry->getGeometryTypeId()) {
    case geos::geom::GEOS_POINT:
        draw((const geos::geom::Point*)transformedGeometry.get());
        break;
    case geos::geom::GEOS_LINESTRING:
        draw((const geos::geom::LineString*)transformedGeometry.get());
        break;
    case geos::geom::GEOS_POLYGON:
        draw((const geos::geom::Polygon*)transformedGeometry.get());
        break;
    case geos::geom::GEOS_MULTIPOINT:
    case geos::geom::GEOS_MULTILINESTRING:
    case geos::geom::GEOS_MULTIPOLYGON:
    case geos::geom::GEOS_GEOMETRYCOLLECTION:
        draw((const geos::geom::GeometryCollection*)transformedGeometry.get());
        break;
    default:
        // 处理其他类型的几何对象，例如 GeometryCollection 等
        break;
    }
}



void MapCanvas::draw(const geos::geom::Point* geom) {
    // std::cerr << "draw point" << std::endl;
    // std::cerr << "Point: " << geom->getX() << ", " << geom->getY() << std::endl;
    _defaultPointSymbol.setDotsPerMM(_dotsPerMM);
    cairo_surface_t* symsurface = _defaultPointSymbol.cairoSurface();
    double symwidth = _defaultPointSymbol.getWidth() * _defaultPointSymbol.getDotsPerMM();
    double symheight = _defaultPointSymbol.getHeight() * _defaultPointSymbol.getDotsPerMM();
    cairo_save(_cairo);
    cairo_set_source_surface(_cairo, symsurface, geom->getX() - symwidth * 0.5, geom->getY() - symheight * 0.5);
    cairo_rectangle(_cairo, geom->getX() - symwidth * 0.5, geom->getY() - symheight * 0.5, symwidth, symheight);
    cairo_clip(_cairo);
    cairo_paint(_cairo);
    cairo_restore(_cairo);
    cairo_surface_destroy(symsurface);
}

void MapCanvas::setStrokeStyle(SymShape* shp) {
    const SymStroke* stroke = shp->stroke();
    const SymColor& color = stroke->color();
    cairo_set_source_rgba(_cairo, color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0, color.alpha() / 255.0);
    double linewidth = round(stroke->width() * _dotsPerMM);
    linewidth = std::max(1.0, linewidth);
    cairo_set_line_width(_cairo, linewidth);

    if (stroke->cap() == SymStroke::CAP_ROUND) {
        cairo_set_line_cap(_cairo, CAIRO_LINE_CAP_ROUND);
    }
    else if (stroke->cap() == SymStroke::CAP_SQUARE) {
        cairo_set_line_cap(_cairo, CAIRO_LINE_CAP_SQUARE);
    }
    else if (stroke->cap() == SymStroke::CAP_BUTT) {
        cairo_set_line_cap(_cairo, CAIRO_LINE_CAP_BUTT);
    }

    if (stroke->join() == SymStroke::JOIN_ROUND) {
        cairo_set_line_join(_cairo, CAIRO_LINE_JOIN_ROUND);
    }
    else if (stroke->join() == SymStroke::JOIN_BEVEL) {
        cairo_set_line_join(_cairo, CAIRO_LINE_JOIN_BEVEL);
    }
    else if (stroke->join() == SymStroke::JOIN_MITER) {
        cairo_set_line_join(_cairo, CAIRO_LINE_JOIN_MITER);
    }

    std::vector<double> dashes = stroke->dashes();
    for (auto& dash : dashes) {
        dash *= _dotsPerMM;
    }
    cairo_set_dash(_cairo, dashes.data(), dashes.size(), 0);

}

void MapCanvas::draw(const geos::geom::LineString* geom, SymSystemLine* symshp) {
    if (!geom || geom->isEmpty()) return;

    cairo_save(_cairo);

    std::unique_ptr<geos::geom::CoordinateSequence> coords = geom->getCoordinates();
    geos::geom::Coordinate& firstCoord = coords->getAt(0);
    cairo_move_to(_cairo, floor(firstCoord.x) + 0.5, floor(firstCoord.y) + 0.5);
    for (size_t i = 1; i < coords->size(); ++i) {
        const geos::geom::Coordinate& coord = coords->getAt(i);
        cairo_line_to(_cairo, floor(coord.x) + 0.5, floor(coord.y) + 0.5);
    }

    setStrokeStyle(symshp);

    cairo_stroke(_cairo);
    cairo_restore(_cairo);


}

void MapCanvas::draw(const geos::geom::LineString* geom) {
    size_t nsymshp = _defaultLineSymbol.getShapeCount();
    for (int i = 0; i < nsymshp; ++i) {
        const SymShape* shp = _defaultLineSymbol.getShape(i);
        if (shp->type() == SymShape::SYM_SYSTEM_LINE) {
            draw(geom, (SymSystemLine*)shp);
        }
    }
}

void MapCanvas::draw(const geos::geom::Polygon* geom) {
    std::cerr << "draw polygon" << std::endl;
}

void MapCanvas::draw(const geos::geom::GeometryCollection* geom) {
    std::cerr << "draw geometrycollection" << std::endl;
}




void MapCanvas::setDefaultPointSymbol(const MapSymbol& symbol) {
    _defaultPointSymbol = symbol;
}
void MapCanvas::setDefaultLineSymbol(const MapSymbol& symbol) {
    _defaultLineSymbol = symbol;
}
void MapCanvas::setDefaultFillSymbol(const MapSymbol& symbol) {
    _defaultFillSymbol = symbol;
}
