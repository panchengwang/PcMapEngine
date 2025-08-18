#ifndef __SYM_CANVAS_H__
#define __SYM_CANVAS_H__

#include <cairo.h>
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
#include "SymSystemFill.h"
#include "SymRegularPolygon.h"
#include "SymStar.h"
#include "SymText.h"


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
    void draw(SymSystemFill* shp);
    void draw(SymRegularPolygon* shp);
    void draw(SymStar* shp);
    void draw(SymText* shp);

    void begin();
    void end();
    char* data(size_t& size);
    cairo_surface_t* cairoSurface() const;
    // sk_sp<SkImage> getImage();
    // sk_sp<SkFontMgr> _fontMgr;
protected:
    void setStrokeStyle(SymShape* shp);
    void setFillStyle(SymShape* shp);

    // sk_sp<SkSurface> _surface;
    // SkCanvas* _canvas;
protected:
    // sk_sp<SkSurface> _surface;
    // SkCanvas* _canvas;
    cairo_surface_t* _surface;
    cairo_t* _canvas;
    double _width;
    double _height;
    double _dotsPerMM;

    cv::Mat _transformMatrix;
};

#endif
