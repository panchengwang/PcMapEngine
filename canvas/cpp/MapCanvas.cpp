#include <MapCanvas.h>
#include <iostream>
#include <algorithm>
#include <uuid.h>
#include <SkData.h>
#include <SkImage.h>
#include <SkStream.h>
#include <SkSurface.h>
#include <SkPath.h>
#include <SkPngEncoder.h>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <filesystem>
#include <opencv4/opencv2/opencv.hpp>
#include <geos/geom/util/GeometryEditor.h>


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
    _canvas = NULL;
    setFormat("PNG");

    // 创建一个临时文件路径，并将其赋值给_filename变量。
    _filename = createUUID();
    _geomFactory = geos::geom::GeometryFactory::create();
    _geomEditor = geos::geom::util::GeometryEditor(_geomFactory.get());
    _affineOperator.setGeometryFactory(_geomFactory.get());
    _wktReader = geos::io::WKTReader(_geomFactory.get());

    if (!_defaultStrokeSymbol.fromJson(R"({
        "width": 10.0,
        "height":10.0,
        "shapes":[{
            "type":"circle",
            "stroke":{
                "color":"#000000",
                "width":1.0,
                "cap":"round",
                "join":"miter",
                "dashes":[10,0]
            },
            "fill":{
            },
            "center":[0.12334433434343, 0.0],
            "radius":0.5
        }]
    })")) {
        std::cerr << _defaultStrokeSymbol.getErrorMessage() << std::endl;
    }
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
    _canvas = NULL;
    setFormat("PNG");
    _filename = createUUID();
    _geomFactory = geos::geom::GeometryFactory::create();
    _geomEditor = geos::geom::util::GeometryEditor(_geomFactory.get());
    _affineOperator.setGeometryFactory(_geomFactory.get());
    _wktReader = geos::io::WKTReader(_geomFactory.get());
}


MapCanvas::~MapCanvas() {

}

void MapCanvas::recalculateMapParameters() {
    double sx = _width / (_maxX - _minX);
    double sy = _height / (_maxY - _minY);
    _scale = (sx < sy) ? sx : sy;

    _minX = _centerX - (_width * 0.5) * _scale;
    _maxX = _centerX + (_width * 0.5) * _scale;
    _minY = _centerY - (_height * 0.5) * _scale;
    _maxY = _centerY + (_height * 0.5) * _scale;


    std::vector<cv::Point2f> srcPoints = {
        cv::Point2f(_minX, _maxY),    // 左上
        cv::Point2f(_minX, _minY),   // 左下
        cv::Point2f(_maxX, _minY)  // 右下
    };

    std::vector<cv::Point2f> dstPoints = {
        cv::Point2f(0, 0),   // 变换后左上
        cv::Point2f(0, _height * _dotsPerMM),  // 变换后左下
        cv::Point2f(_width * _dotsPerMM, _height * _dotsPerMM)  // 变换后右下
    };
    cv::Mat transformMatrix = cv::getAffineTransform(srcPoints, dstPoints);
    _transformMatrix = transformMatrix;

    _affineOperator.setMatirx(_transformMatrix);
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
        _surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(_width * _dotsPerMM, _height * _dotsPerMM));
        _canvas = _surface->getCanvas();
    }
    else if (_format == "pdf") {
        // _canvas = SkCanvas::MakePDF(_filename.c_str(), _width * _dotsPerMM, _height * _dotsPerMM);
        return false;
    }
    else {
        return false;
    }


    if (_canvas != NULL) {
        _canvas->clear(SK_ColorTRANSPARENT);
        // _canvas->save();

    }

    return true;
}



geos::geom::Geometry::Ptr MapCanvas::mapToCanvas(const geos::geom::Geometry* geom) {

}

bool MapCanvas::end() {
    if (_canvas != NULL) {
        // _canvas->restore();
    }
    return true;
}



void MapCanvas::log() {
    std::cerr << "MapCanvas:    minx: " << _minX << ", maxx: " << _maxX << ", miny: " << _minY << ", maxy: " << _maxY << std::endl;
    std::cerr << "MapCanvas:    centerx: " << _centerX << ", centery: " << _centerY << std::endl;
    std::cerr << "MapCanvas:    width: " << _width << ", height: " << _height << std::endl;
    std::cerr << "MapCanvas:    scale: " << _scale << std::endl;
    std::cerr << "MapCanvas:    dotsPerMM: " << _dotsPerMM << std::endl;
    std::cerr << "MapCanvas:    format: " << _format << std::endl;
    std::cerr << "MapCanvas:    filename: " << _filename << std::endl;

    std::cerr << "[ "
        << _transformMatrix.at<double>(0, 0) << ", "
        << _transformMatrix.at<double>(0, 1) << ", "
        << _transformMatrix.at<double>(0, 2) << " ]\n";

    std::cerr << "[ "
        << _transformMatrix.at<double>(1, 0) << ", "
        << _transformMatrix.at<double>(1, 1) << ", "
        << _transformMatrix.at<double>(1, 2) << " ]"
        << std::endl;

    std::cerr << _defaultStrokeSymbol.toJson() << std::endl;
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


/**
 * @brief 获取画布数据
 *
 * 根据画布格式获取画布数据。支持 PNG 和 PDF 格式。
 *
 * @param len 数据长度，以字节为单位
 * @return 返回画布数据的指针，如果无法获取数据则返回 NULL.
 *  需要调用者负责释放返回的内存。
 */
char* MapCanvas::data(size_t& len) {
    char* buf = NULL;
    len = 0;
    if (_format == "png") {
        sk_sp<SkImage> img(_surface->makeImageSnapshot());
        if (!img) {
            return NULL;
        }
        sk_sp<SkData> png = SkPngEncoder::Encode(nullptr, img.get(), {});
        if (!png) {
            return NULL;
        }
        len = png->size();
        buf = new char[len];
        memcpy(buf, png->data(), len);
    }
    else if (_format == "pdf") {
        // return _canvas->pdfDocument()->data();
    }

    return buf;
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

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(SK_ColorRED);
    paint.setStyle(SkPaint::kFill_Style);

    SkPath path;
    path.addCircle(geom->getX(), geom->getY(), 30);
    _canvas->drawPath(path, paint);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setColor(SK_ColorBLUE);
    paint.setStrokeWidth(1);
    _canvas->drawPath(path, paint);

    // _canvas->drawCircle(geom->getX(), geom->getY(), 1.0f, SkPaint());
}

void MapCanvas::draw(const geos::geom::LineString* geom) {
    std::cerr << "draw linestring" << std::endl;
}

void MapCanvas::draw(const geos::geom::Polygon* geom) {
    std::cerr << "draw polygon" << std::endl;
}

void MapCanvas::draw(const geos::geom::GeometryCollection* geom) {
    std::cerr << "draw geometrycollection" << std::endl;
}
