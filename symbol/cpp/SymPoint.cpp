#include "SymPoint.h"
#include "JsonUtils.h"


SymPoint::SymPoint() {
    _x = _y = 0.0f;
}

SymPoint::SymPoint(double x, double y) {
    _x = x;
    _y = y;
}


SymPoint::~SymPoint() {

}



bool SymPoint::fromJson(json_object* jsonObj, std::string& errMsg) {
    if (json_object_get_type(jsonObj) == json_type_array) {
        if (json_object_array_length(jsonObj) >= 2) {
            _x = json_object_get_double(json_object_array_get_idx(jsonObj, 0));
            _y = json_object_get_double(json_object_array_get_idx(jsonObj, 1));
        }
        else {
            errMsg = "Invalid JSON format for SymPoint";
            return false;
        }
    }
    else    if (json_object_get_type(jsonObj) != json_type_object) {
        JSON_GET_DOUBLE(jsonObj, "x", _x, errMsg);
        JSON_GET_DOUBLE(jsonObj, "y", _y, errMsg);
    }
    else {
        errMsg = "Invalid JSON format for SymPoint";
        return false;
    }

    return true;
}


json_object* SymPoint::toJson() const {
    json_object* jsonObj = json_object_new_array();
    json_object_array_add(jsonObj, json_object_new_double(_x));
    json_object_array_add(jsonObj, json_object_new_double(_y));
    return jsonObj;
}



SymPoint SymPoint::transform(const cv::Mat& transformMatrix) {
    // cv::Mat points = (cv::Mat_<float>(1, 2) <<
    //     _x, _y
    //     );

    // cv::Mat transformed_points;
    // cv::transform(points, transformed_points, transformMatrix);
    // return SymPoint(transformed_points.at<float>(0, 0), transformed_points.at<float>(0, 1));

    double x = _x * transformMatrix.at<double>(0, 0) + _y * transformMatrix.at<double>(0, 1) + transformMatrix.at<double>(0, 2);
    double y = _x * transformMatrix.at<double>(1, 0) + _y * transformMatrix.at<double>(1, 1) + transformMatrix.at<double>(1, 2);
    return SymPoint(x, y);

}