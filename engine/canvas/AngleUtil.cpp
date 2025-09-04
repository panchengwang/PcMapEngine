#include "AngleUtil.h"


double AngleUtil::radianToDegree(double radian) {
    return (180.0 / M_PI) * radian;
}


double AngleUtil::degreeToRadian(double degree) {
    return (M_PI / 180.0) * degree;
}


double AngleUtil::vectorAngle(const Eigen::Vector2d& v) {
    return std::atan2(v.y(), v.x());
}


double AngleUtil::angleBisectorDirection(
    const Eigen::Vector2d& A,
    const Eigen::Vector2d& B,
    const Eigen::Vector2d& C) {
    Eigen::Vector2d BA = A - B;
    Eigen::Vector2d BC = C - B;

    // 归一化向量
    Eigen::Vector2d u = BA.normalized();
    Eigen::Vector2d v = BC.normalized();

    // 计算角平分线方向向量（单位向量的和）
    Eigen::Vector2d bisector_dir = u + v;

    // 处理特殊情况：当两个向量方向相反时（角为180度）
    if (bisector_dir.norm() < 1e-10) {
        // 此时角平分线有两个可能方向，选择与BA垂直的方向
        // 这里选择逆时针旋转90度的方向
        Eigen::Vector2d perpendicular(-BA.y(), BA.x());
        return AngleUtil::vectorAngle(perpendicular.normalized());
    }

    // 计算角平分线的方向角
    return AngleUtil::vectorAngle(bisector_dir);
}


double AngleUtil::normalizeAngle(double angle_rad) {
    double normalized = std::fmod(angle_rad, 2 * M_PI);
    if (normalized < 0) {
        normalized += 2 * M_PI;
    }
    return normalized;
}