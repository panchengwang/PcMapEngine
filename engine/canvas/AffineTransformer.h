#ifndef __AFFINE_TRANSFORMER_H__
#define __AFFINE_TRANSFORMER_H__ 

class AffineTransformer {
public:
    AffineTransformer();
    ~AffineTransformer();

    bool from3ControlPoints(
        double srcX1, double srcY1, double srcX2, double srcY2, double srcX3, double srcY3,
        double dstX1, double dstY1, double dstX2, double dstY2, double dstX3, double dstY3);

    void setMatrix(double a, double b, double c, double d, double e, double f);

    double a() const;
    double b() const;
    double c() const;
    double d() const;
    double e() const;
    double f() const;

protected:
    double _a, _b, _c, _d, _e, _f;  // Affine transformation coefficients

};

#endif
