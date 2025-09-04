#ifndef __ANGLE_UTIL_H__
#define __ANGLE_UTIL_H__

#include <cmath>
#include <Eigen/Dense>

class AngleUtil {
public:
    static double radianToDegree(double radian);
    static double degreeToRadian(double degree);

    static double vectorAngle(const Eigen::Vector2d& v);
    static double angleBisectorDirection(
        const Eigen::Vector2d& A,
        const Eigen::Vector2d& B,
        const Eigen::Vector2d& C);
    static double normalizeAngle(double angle_rad);
};



#endif
