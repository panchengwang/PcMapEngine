#ifndef __SYM_CANVAS_H__
#define __SYM_CANVAS_H__

#include <SkCanvas.h>
#include <SkSurface.h>
#include "SymShape.h"
#include "SymCircle.h"
#include "SymRectangle.h"
#include "SymPolygon.h"
#include "SymLineString.h"
#include "SymEllipse.h"
#include "SymArc.h"
#include "SymChord.h"
#include "SymPie.h"
#include "SymSystemLine.h"


class SymCanvas
{
public:
    SymCanvas();
    ~SymCanvas();

    void set(double width, double height, double dotsPerMM);
    void draw(SymShape* shp);
    void draw(SymCircle* shp);
    void draw(SymRectangle* shp);
    void draw(SymPolygon* shp);
    void draw(SymLineString* shp);
    void draw(SymEllipse* shp);
    void draw(SymArc* shp);
    void draw(SymChord* shp);
    void draw(SymPie* shp);
    void draw(SymSystemLine* shp);

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
