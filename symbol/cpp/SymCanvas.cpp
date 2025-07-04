#include "SymCanvas.h"
#include <vector>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include "SymCircle.h"
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
     cv::Point2f(-_width * 0.5, _height * 0.5),    // 左上
     cv::Point2f(-_width * 0.5, -_height * 0.5),   // 左下
     cv::Point2f(_width * 0.5, -_height * 0.5)  // 右下
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

    default:
        break;
    }
}


void SymCanvas::draw(SymCircle* shp) {
    // std::cerr << "draw circle " << std::endl;
    SymPoint center = shp->center().transform(_transformMatrix);
    SkPaint paint = shp->stroke()->toPaint(_dotsPerMM);


    SkPath path;
    path.addOval(
        SkRect::MakeLTRB(
            center.x() - shp->radius() * _dotsPerMM * _width * 0.5,
            center.y() - shp->radius() * _dotsPerMM * _height * 0.5,
            center.x() + shp->radius() * _dotsPerMM * _width * 0.5,
            center.y() + shp->radius() * _dotsPerMM * _height * 0.5)
    );

    // path.addCircle(center.x(), center.y(), shp->radius() * _dotsPerMM);
    _canvas->drawPath(path, paint);
    // paint.setStyle(SkPaint::kStroke_Style);
    // paint.setColor(SK_ColorBLUE);
    // paint.setStrokeWidth(1);
    // _canvas->drawPath(path, paint);

    // _canvas->drawCircle(geom->getX(), geom->getY(), 1.0f, SkPaint());
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