#ifndef __SYM_POINT_H__
#define __SYM_POINT_H__

#include <iostream>
#include <string>
#include <vector>
#include <json.h>


class SymPoint {
public:
    SymPoint();
    ~SymPoint();

    bool fromJson(json_object* jsonObj, std::string& errMsg);
    json_object* toJson() const;
private:
    double _x, _y;

};

#endif // __SYM_POINT_H__