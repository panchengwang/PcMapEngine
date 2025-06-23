#include "AffineTransformOperation.h"

void AffineTransformOperation::setMatirx(cv::Mat& matrix) {
    this->a = matrix.at<double>(0, 0);
    this->b = matrix.at<double>(0, 1);
    this->e = matrix.at<double>(0, 2);
    this->c = matrix.at<double>(1, 0);
    this->d = matrix.at<double>(1, 1);
    this->f = matrix.at<double>(1, 2);
}


AffineTransformOperation::AffineTransformOperation() {
    a = 1.0;
    b = 0.0;
    c = 0.0;
    d = 1.0;
    e = 0.0;
    f = 0.0;
}



AffineTransformOperation::AffineTransformOperation(double a, double b, double c, double d, double e, double f)
    : a(a), b(b), c(c), d(d), e(e), f(f) {
}


void AffineTransformOperation::setGeometryFactory(geos::geom::GeometryFactory* factory) {
    _factory = factory;
}

// 重写编辑方法
std::unique_ptr< Geometry > AffineTransformOperation::edit(const geos::geom::Geometry* geometry,
    const geos::geom::GeometryFactory* factory) {
    // 仅处理点、线串和线性环
    if (geometry->getGeometryTypeId() == geos::geom::GEOS_POINT ||
        geometry->getGeometryTypeId() == geos::geom::GEOS_LINESTRING ||
        geometry->getGeometryTypeId() == geos::geom::GEOS_LINEARRING) {

        // 获取坐标序列
        auto coords = geometry->getCoordinates();
        auto newCoords = coords->clone();
        // auto newCoords = factory->getCoordinateSequenceFactory()->create(
        //     static_cast<size_t>(coords->size()), coords->getDimension());

        // 应用放射变换到每个坐标
        for (size_t i = 0; i < static_cast<size_t>(coords->size()); i++) {
            double x = coords->getX(i);
            double y = coords->getY(i);

            // 矩阵变换: [a b e] [x]   [a*x + b*y + e]
            //          [c d f] [y] = [c*x + d*y + f]
            double newX = a * x + b * y + e;
            double newY = c * x + d * y + f;

            newCoords->setAt(geos::geom::Coordinate(newX, newY), i);
        }

        // 创建新几何对象
        if (geometry->getGeometryTypeId() == geos::geom::GEOS_POINT) {
            return _factory->createPoint(*newCoords);
        }
        else if (geometry->getGeometryTypeId() == geos::geom::GEOS_LINESTRING) {
            return _factory->createLineString(*newCoords);
        }
        else if (geometry->getGeometryTypeId() == geos::geom::GEOS_LINEARRING) {
            return _factory->createLinearRing(*newCoords);
        }
    }

    // 对于其他类型几何，返回原始几何（编辑器会递归处理子组件）
    return geometry->clone();
}