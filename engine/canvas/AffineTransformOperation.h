#ifndef __AFFINETRANSFORMOPERATION_H
#define __AFFINETRANSFORMOPERATION_H

#include <map_engine_dll_exports.h>
#include <opencv4/opencv2/opencv.hpp>
#include <geos.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/util/GeometryEditorOperation.h>
#include <opencv4/opencv2/opencv.hpp>
#include <iostream>

class DLL_EXPORT AffineTransformOperation : public geos::geom::util::GeometryEditorOperation {
private:
    double _a, _b, _c, _d, _e, _f;
    geos::geom::GeometryFactory* _factory;
public:

    AffineTransformOperation();
    AffineTransformOperation(double a, double b, double c, double d, double e, double f);
    void setGeometryFactory(geos::geom::GeometryFactory* factory);

    void setMatirx(cv::Mat& matrix);
    void setMatrix(double a, double b, double c, double d, double e, double f);
    // 重写编辑方法
    std::unique_ptr< Geometry > edit(const geos::geom::Geometry* geometry,
        const geos::geom::GeometryFactory* factory) override;

    void log() {
        std::cerr << "AffineTransformOperation: "
            << "a: " << _a << ", b: " << _b
            << ", c: " << _c << ", d: " << _d
            << ", e: " << _e << ", f: " << _f
            << std::endl;
    }
};

#endif
