#ifndef __SYM_CANVAS_H__
#define __SYM_CANVAS_H__

#include <SkCanvas.h>
#include <SkSurface.h>


class SymCanvas
{
public:
    SymCanvas();
    ~SymCanvas();

    void set(double width, double height, double dotsPerMM);


protected:
    sk_sp<SkSurface> _surface;
    SkCanvas* _canvas;

    double _width;
    double _height;
    double _dotsPerMM;


};

#endif