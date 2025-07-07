#include "SymCanvas.h"
#include <vector>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include "SymCircle.h"
#include "SymRectangle.h"
#include <SkPaint.h>
#include <SkPath.h>


SymCanvas::SymCanvas() {
    set(10, 10, 96.0 / 25.4);
}


SymCanvas::~SymCanvas() {

}


void SymCanvas::set(double width, double height, double dotsPerMM) {
    _width = width;
    _height = height;
    _dotsPerMM = dotsPerMM;

    std::vector<cv::Point2f> srcPoints = {
     cv::Point2f(-1.0, 1.0),    // 左上
     cv::Point2f(-1.0, -1.0),   // 左下
     cv::Point2f(1.0, -1.0)  // 右下
    };

    std::vector<cv::Point2f> dstPoints = {
        cv::Point2f(0, 0),   // 变换后左上
        cv::Point2f(0, _height * _dotsPerMM),  // 变换后左下
        cv::Point2f(_width * _dotsPerMM, _height * _dotsPerMM)  // 变换后右下
    };
    _transformMatrix = cv::getAffineTransform(srcPoints, dstPoints);

}


void SymCanvas::draw(SymShape* shp) {
    switch (shp->type()) {
    case SymShape::SYM_CIRCLE:
        draw(static_cast<SymCircle*>(shp));
        break;
    case SymShape::SYM_RECTANGLE:
        draw(static_cast<SymRectangle*>(shp));
        break;
    default:
        break;
    }
}


void SymCanvas::draw(SymCircle* shp) {
    SymPoint center = shp->center().transform(_transformMatrix);
    SkPath path;
    path.addOval(
        SkRect::MakeLTRB(
            center.x() - shp->radius() * _dotsPerMM * _width * 0.5,
            center.y() - shp->radius() * _dotsPerMM * _height * 0.5,
            center.x() + shp->radius() * _dotsPerMM * _width * 0.5,
            center.y() + shp->radius() * _dotsPerMM * _height * 0.5)
    );
    SkPaint paint;
    paint = shp->fill()->toPaint(_dotsPerMM);
    _canvas->drawPath(path, paint);

    paint = shp->stroke()->toPaint(_dotsPerMM);
    _canvas->drawPath(path, paint);

}

void SymCanvas::draw(SymRectangle* shp) {
    SymPoint lb = SymPoint(shp->minX(), shp->minY()).transform(_transformMatrix);
    SymPoint rt = SymPoint(shp->maxX(), shp->maxY()).transform(_transformMatrix);
    SkPath path;
    path.addRect(SkRect::MakeLTRB(
        lb.x() , lb.y() ,
        rt.x() , rt.y() ));
    SkPaint paint;
    paint = shp->fill()->toPaint(_dotsPerMM);
    _canvas->drawPath(path, paint);

    paint = shp->stroke()->toPaint(_dotsPerMM);
    _canvas->drawPath(path, paint);
}


void SymCanvas::begin() {
    _surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(_width * _dotsPerMM, _height * _dotsPerMM));
    _canvas = _surface->getCanvas();
    _canvas->clear(SK_ColorTRANSPARENT);
}

void SymCanvas::end() {

}


sk_sp<SkImage> SymCanvas::getImage() {
    return _surface->makeImageSnapshot();
}
