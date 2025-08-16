#include "AffineTransformer.h"
#include <Eigen/Dense>  
#include <vector>

using namespace Eigen;

AffineTransformer::AffineTransformer() {
    // Constructor implementation
}

AffineTransformer::~AffineTransformer() {
    // Destructor implementation
}


bool AffineTransformer::from3ControlPoints(
    double srcX1, double srcY1, double srcX2, double srcY2, double srcX3, double srcY3,
    double dstX1, double dstY1, double dstX2, double dstY2, double dstX3, double dstY3) {

    MatrixXf A(6, 6);
    VectorXf B(6);
    A << srcX1, srcY1, 1, 0, 0, 0,
        0, 0, 0, srcX1, srcY1, 1,
        srcX2, srcY2, 1, 0, 0, 0,
        0, 0, 0, srcX2, srcY2, 1,
        srcX3, srcY3, 1, 0, 0, 0,
        0, 0, 0, srcX3, srcY3, 1;
    B[0] = dstX1;
    B[1] = dstY1;
    B[2] = dstX2;
    B[3] = dstY2;
    B[4] = dstX3;
    B[5] = dstY3;
    VectorXf X = A.colPivHouseholderQr().solve(B);
    _a = X[0];
    _b = X[1];
    _c = X[3];
    _d = X[4];
    _e = X[2];
    _f = X[5];
    return true;
}


void AffineTransformer::setMatrix(double a, double b, double c, double d, double e, double f) {
    this->_a = a;
    this->_b = b;
    this->_c = c;
    this->_d = d;
    this->_e = e;
    this->_f = f;
}

double AffineTransformer::a() const {
    return _a;
}

double AffineTransformer::b() const {
    return _b;
}

double AffineTransformer::c() const {
    return _c;
}

double AffineTransformer::d() const {
    return _d;
}

double AffineTransformer::e() const {
    return _e;
}

double AffineTransformer::f() const {
    return _f;
}
