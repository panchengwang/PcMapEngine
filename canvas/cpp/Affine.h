#ifndef __AFFINE_H
#define __AFFINE_H

#include <map_engine_dll_exports.h>
#include <opencv4/opencv2/opencv.hpp>
#include <geos.h>
#include <geos/geom/util/GeometryEditorOperation.h>
#include <opencv4/opencv2/opencv.hpp>

class AffineTransformOperation : public geos::geom::util::GeometryEditorOperation {
private:
    double a, b, c, d, e, f;

public:

    AffineTransformOperation();
    AffineTransformOperation(double a, double b, double c, double d, double e, double f);
    void setMatirx(cv::Mat& matrix);
    // 重写编辑方法
    std::unique_ptr< Geometry > edit(const geos::geom::Geometry* geometry,
        const geos::geom::GeometryFactory* factory) override;
};

#endif
