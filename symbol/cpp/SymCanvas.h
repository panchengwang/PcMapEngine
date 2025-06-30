#ifndef __SYM_CANVAS_H__
#define __SYM_CANVAS_H__

#include <SkCanvas.h>
#include <SkSurface.h>
#include "SymShape.h"
#include "SymCircle.h"


class SymCanvas
{
public:
    SymCanvas();
    ~SymCanvas();

    void set(double width, double height, double dotsPerMM);
    void draw(SymShape* shp);
    void draw(SymCircle* shp);

    void begin();
    void end();
    sk_sp<SkImage> getImage();
protected:
    sk_sp<SkSurface> _surface;
    SkCanvas* _canvas;

    double _width;
    double _height;
    double _dotsPerMM;

    cv::Mat _transformMatrix;
};

#endif