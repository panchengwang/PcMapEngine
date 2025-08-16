#include "AffineTransformOperation.h"

void AffineTransformOperation::setMatirx(cv::Mat& matrix) {
    this->_a = matrix.at<double>(0, 0);
    this->_b = matrix.at<double>(0, 1);
    this->_e = matrix.at<double>(0, 2);
    this->_c = matrix.at<double>(1, 0);
    this->_d = matrix.at<double>(1, 1);
    this->_f = matrix.at<double>(1, 2);
}


void AffineTransformOperation::setMatrix(double a, double b, double c, double d, double e, double f) {
    this->_a = a;
    this->_b = b;
    this->_c = c;
    this->_d = d;
    this->_e = e;
    this->_f = f;
}

AffineTransformOperation::AffineTransformOperation() {
    _a = 1.0;
    _b = 0.0;
    _c = 0.0;
    _d = 1.0;
    _e = 0.0;
    _f = 0.0;
}



AffineTransformOperation::AffineTransformOperation(double a, double b, double c, double d, double e, double f)
    : _a(a), _b(b), _c(c), _d(d), _e(e), _f(f) {
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
            double newX = _a * x + _b * y + _e;
            double newY = _c * x + _d * y + _f;

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