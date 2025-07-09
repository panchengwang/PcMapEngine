#ifndef __SYM_POINT_H__
#define __SYM_POINT_H__

#include <iostream>
#include <string>
#include <vector>
#include <json-c/json.h>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>



class SymPoint {
public:
    SymPoint();
    SymPoint(double x, double y);
    ~SymPoint();

    bool fromJson(json_object* jsonObj, std::string& errMsg);
    json_object* toJson() const;

    double x() const { return _x; }
    void setX(double x) { _x = x; }

    double y() const { return _y; }
    void setY(double y) { _y = y; }

    SymPoint transform(const cv::Mat& transformMatrix) const;
private:
    double _x, _y;

};

#endif // __SYM_POINT_H__