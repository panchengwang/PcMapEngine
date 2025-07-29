#include "SymCanvas.h"
#include "../SymMath.h"

#include <vector>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include "SymCircle.h"
#include "SymRectangle.h"
#include "SymSolidFill.h"


SymCanvas::SymCanvas() {
    set(10, 10, 96.0 / 25.4);
    _canvas = NULL;
    _surface = NULL;
}


SymCanvas::~SymCanvas() {
    if (_canvas) {
        cairo_destroy(_canvas);
        _canvas = NULL;
    }

    if (_surface) {
        cairo_surface_destroy(_surface);
        _surface = NULL;
    }

}


void SymCanvas::set(double width, double height, double dotsPerMM) {
    _width = width;
    _height = height;
    _dotsPerMM = dotsPerMM;

    std::vector<cv::Point2f> srcPoints = {
     cv::Point2f(-1.0, 1.0),    // 左上
     cv::Point2f(-1.0, -1.0),   // 左下
     cv::Point2f(1.0, -1.0)  // 右下
    };

    std::vector<cv::Point2f> dstPoints = {
        cv::Point2f(0, 0),   // 变换后左上
        cv::Point2f(0, _height * _dotsPerMM),  // 变换后左下
        cv::Point2f(_width * _dotsPerMM, _height * _dotsPerMM)  // 变换后右下
    };
    _transformMatrix = cv::getAffineTransform(srcPoints, dstPoints);

}


void SymCanvas::draw(SymShape* shp) {
    switch (shp->type()) {
    case SymShape::SYM_CIRCLE:
        draw(static_cast<SymCircle*>(shp));
        break;
    case SymShape::SYM_RECTANGLE:
        draw(static_cast<SymRectangle*>(shp));
        break;
    case SymShape::SYM_POLYGON:
        draw(static_cast<SymPolygon*>(shp));
        break;
    case SymShape::SYM_LINESTRING:
        draw(static_cast<SymLineString*>(shp));
        break;
    case SymShape::SYM_ELLIPSE:
        draw(static_cast<SymEllipse*>(shp));
        break;
    case SymShape::SYM_ARC:
        draw(static_cast<SymArc*>(shp));
        break;
    case SymShape::SYM_CHORD:
        draw(static_cast<SymChord*>(shp));
        break;
    case SymShape::SYM_PIE:
        draw(static_cast<SymPie*>(shp));
        break;
    case SymShape::SYM_SYSTEM_LINE:
        draw(static_cast<SymSystemLine*>(shp));
        break;
    case SymShape::SYM_REGULAR_POLYGON:
        draw(static_cast<SymRegularPolygon*>(shp));
        break;
    case SymShape::SYM_STAR:
        draw(static_cast<SymStar*>(shp));
        break;
    case SymShape::SYM_TEXT:
        draw(static_cast<SymText*>(shp));
        break;
    default:
        break;
    }
}

void SymCanvas::setStrokeStyle(SymShape* shp) {
    SymColor color = shp->_stroke->_color;
    cairo_set_source_rgba(_canvas, color._red / 255.0, color._green / 255.0, color._blue / 255.0, color._alpha / 255.0);
    cairo_set_line_width(_canvas, shp->_stroke->_width * _dotsPerMM);

    if (shp->_stroke->_cap == SymStroke::CAP_ROUND) {
        cairo_set_line_cap(_canvas, CAIRO_LINE_CAP_ROUND);
    }
    else if (shp->_stroke->_cap == SymStroke::CAP_SQUARE) {
        cairo_set_line_cap(_canvas, CAIRO_LINE_CAP_SQUARE);
    }
    else if (shp->_stroke->_cap == SymStroke::CAP_BUTT) {
        cairo_set_line_cap(_canvas, CAIRO_LINE_CAP_BUTT);
    }

    if (shp->_stroke->_join == SymStroke::JOIN_ROUND) {
        cairo_set_line_join(_canvas, CAIRO_LINE_JOIN_ROUND);
    }
    else if (shp->_stroke->_join == SymStroke::JOIN_BEVEL) {
        cairo_set_line_join(_canvas, CAIRO_LINE_JOIN_BEVEL);
    }
    else if (shp->_stroke->_join == SymStroke::JOIN_MITER) {
        cairo_set_line_join(_canvas, CAIRO_LINE_JOIN_MITER);
    }

    std::vector<double> dashes = shp->_stroke->_dashes;
    for (auto& dash : dashes) {
        dash *= _dotsPerMM;
    }
    cairo_set_dash(_canvas, dashes.data(), dashes.size(), 0);

}


void SymCanvas::setFillStyle(SymShape* shp) {
    if (shp->_fill->_type == SymFill::FILL_SOLID) {
        SymSolidFill* fill = static_cast<SymSolidFill*>(shp->_fill);
        cairo_set_source_rgba(_canvas,
            fill->_color._red / 255.0,
            fill->_color._green / 255.0,
            fill->_color._blue / 255.0,
            fill->_color._alpha / 255.0);
    }

}

void SymCanvas::draw(SymCircle* shp) {
    SymPoint center = shp->center().transform(_transformMatrix);

    cairo_save(_canvas);

    cairo_arc(_canvas, center.x(), center.y(), shp->radius() * _dotsPerMM * _width * 0.5, 0, 2 * M_PI);

    setFillStyle(shp);
    cairo_fill_preserve(_canvas);
    setStrokeStyle(shp);
    cairo_stroke(_canvas);
    cairo_restore(_canvas);
    // SkPath path;
    // path.addOval(
    //     SkRect::MakeLTRB(
    //         center.x() - shp->radius() * _dotsPerMM * _width * 0.5,
    //         center.y() - shp->radius() * _dotsPerMM * _height * 0.5,
    //         center.x() + shp->radius() * _dotsPerMM * _width * 0.5,
    //         center.y() + shp->radius() * _dotsPerMM * _height * 0.5)
    // );
    // SkPaint paint;
    // paint = shp->fill()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);

    // paint = shp->stroke()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);

}

void SymCanvas::draw(SymRectangle* shp) {
    SymPoint lb = SymPoint(shp->minX(), shp->minY()).transform(_transformMatrix);
    SymPoint rt = SymPoint(shp->maxX(), shp->maxY()).transform(_transformMatrix);

    cairo_save(_canvas);

    cairo_rectangle(_canvas, lb.x(), lb.y(), rt.x() - lb.x(), rt.y() - lb.y());
    setFillStyle(shp);
    cairo_fill_preserve(_canvas);
    setStrokeStyle(shp);
    cairo_stroke(_canvas);
    cairo_restore(_canvas);

    // SkPath path;
    // path.addRect(SkRect::MakeLTRB(
    //     lb.x(), lb.y(),
    //     rt.x(), rt.y()));
    // SkPaint paint;
    // paint = shp->fill()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);

    // paint = shp->stroke()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
}

void SymCanvas::draw(SymPolygon* shp) {

    const std::vector<SymPoint>& points = shp->points();
    std::vector<SymPoint> transformedPoints;
    for (auto& pt : points) {
        transformedPoints.push_back(pt.transform(_transformMatrix));
    }
    if (transformedPoints.size() < 2) return;

    cairo_save(_canvas);
    cairo_move_to(_canvas, transformedPoints[0].x(), transformedPoints[0].y());
    for (int i = 1; i < transformedPoints.size(); ++i) {
        cairo_line_to(_canvas, transformedPoints[i].x(), transformedPoints[i].y());
    }
    cairo_close_path(_canvas);
    setFillStyle(shp);
    cairo_fill_preserve(_canvas);
    setStrokeStyle(shp);
    cairo_stroke(_canvas);
    cairo_restore(_canvas);
    // SkPath path;
    // path.moveTo(transformedPoints[0].x(), transformedPoints[0].y());
    // for (int i = 1; i < transformedPoints.size(); ++i) {
    //     path.lineTo(transformedPoints[i].x(), transformedPoints[i].y());
    // }
    // path.close();
    // SkPaint paint;
    // paint = shp->fill()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
    // paint = shp->stroke()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
}


void SymCanvas::draw(SymLineString* shp) {

    const std::vector<SymPoint>& points = shp->points();
    std::vector<SymPoint> transformedPoints;
    for (auto& pt : points) {
        transformedPoints.push_back(pt.transform(_transformMatrix));
    }
    if (transformedPoints.size() < 2) return;
    cairo_save(_canvas);
    cairo_move_to(_canvas, transformedPoints[0].x(), transformedPoints[0].y());
    for (int i = 1; i < transformedPoints.size(); ++i) {
        cairo_line_to(_canvas, transformedPoints[i].x(), transformedPoints[i].y());
    }
    setStrokeStyle(shp);
    cairo_stroke(_canvas);
    cairo_restore(_canvas);
    // SkPath path;
    // path.moveTo(transformedPoints[0].x(), transformedPoints[0].y());
    // for (int i = 1; i < transformedPoints.size(); ++i) {
    //     path.lineTo(transformedPoints[i].x(), transformedPoints[i].y());
    // }

    // //path.close();

    // SkPaint paint;
    // paint = shp->stroke()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
}


void SymCanvas::draw(SymEllipse* shp) {
    SymPoint center = shp->center().transform(_transformMatrix);


    cairo_save(_canvas);
    cairo_translate(_canvas, center.x(), center.y());
    cairo_rotate(_canvas, -DEG_TO_RAD * shp->rotation());
    cairo_scale(_canvas, shp->radiusX() * _dotsPerMM * _width * 0.5, shp->radiusY() * _dotsPerMM * _height * 0.5);
    cairo_arc(_canvas, 0.0, 0.0, 1.0, 0.0, 2.0 * M_PI);
    cairo_restore(_canvas);

    setFillStyle(shp);
    cairo_fill_preserve(_canvas);
    setStrokeStyle(shp);
    cairo_stroke(_canvas);

    // SkRect rect = SkRect::MakeLTRB(
    //     center.x() - shp->radiusX() * _dotsPerMM * _width * 0.5,
    //     center.y() - shp->radiusY() * _dotsPerMM * _height * 0.5,
    //     center.x() + shp->radiusX() * _dotsPerMM * _width * 0.5,
    //     center.y() + shp->radiusY() * _dotsPerMM * _height * 0.5);
    // SkPath path;
    // path.addOval(
    //     rect
    // );

    // SkMatrix matrix;

    // matrix.setRotate(-shp->rotation());
    // matrix.preTranslate(-center.x(), -center.y());
    // path.transform(matrix);
    // matrix.reset();
    // matrix.setTranslate(center.x(), center.y());
    // path.transform(matrix);

    // SkPaint paint;
    // paint = shp->fill()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);

    // paint = shp->stroke()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
}


void SymCanvas::draw(SymArc* shp) {
    SymPoint center = shp->center().transform(_transformMatrix);

    cairo_save(_canvas);
    cairo_translate(_canvas, center.x(), center.y());
    cairo_rotate(_canvas, -DEG_TO_RAD * shp->rotation());
    cairo_scale(_canvas, shp->radiusX() * _dotsPerMM * _width * 0.5, shp->radiusY() * _dotsPerMM * _height * 0.5);
    cairo_arc_negative(_canvas, 0.0, 0.0, 1.0, -shp->_startAngle * DEG_TO_RAD, -shp->_endAngle * DEG_TO_RAD);
    cairo_restore(_canvas);

    setStrokeStyle(shp);
    cairo_stroke(_canvas);

    // SkRect rect = SkRect::MakeLTRB(
    //     center.x() - shp->radiusX() * _dotsPerMM * _width * 0.5,
    //     center.y() - shp->radiusY() * _dotsPerMM * _height * 0.5,
    //     center.x() + shp->radiusX() * _dotsPerMM * _width * 0.5,
    //     center.y() + shp->radiusY() * _dotsPerMM * _height * 0.5);
    // SkPath path;
    // path.addArc(
    //     rect, shp->startAngle(), shp->startAngle() - shp->endAngle()
    // );

    // SkMatrix matrix;

    // matrix.setRotate(-shp->rotation());
    // matrix.preTranslate(-center.x(), -center.y());
    // path.transform(matrix);
    // matrix.reset();
    // matrix.setTranslate(center.x(), center.y());
    // path.transform(matrix);

    // SkPaint paint;
    // // paint = shp->fill()->toPaint(_dotsPerMM);
    // // _canvas->drawPath(path, paint);

    // paint = shp->stroke()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
}


void SymCanvas::draw(SymChord* shp) {
    SymPoint center = shp->center().transform(_transformMatrix);

    cairo_save(_canvas);
    cairo_translate(_canvas, center.x(), center.y());
    cairo_rotate(_canvas, -DEG_TO_RAD * shp->rotation());
    cairo_scale(_canvas, shp->radiusX() * _dotsPerMM * _width * 0.5, shp->radiusY() * _dotsPerMM * _height * 0.5);
    cairo_arc_negative(_canvas, 0.0, 0.0, 1.0, -shp->_startAngle * DEG_TO_RAD, -shp->_endAngle * DEG_TO_RAD);
    cairo_close_path(_canvas);
    cairo_restore(_canvas);

    setFillStyle(shp);
    cairo_fill_preserve(_canvas);
    setStrokeStyle(shp);
    cairo_stroke(_canvas);

    // SkRect rect = SkRect::MakeLTRB(
    //     center.x() - shp->radiusX() * _dotsPerMM * _width * 0.5,
    //     center.y() - shp->radiusY() * _dotsPerMM * _height * 0.5,
    //     center.x() + shp->radiusX() * _dotsPerMM * _width * 0.5,
    //     center.y() + shp->radiusY() * _dotsPerMM * _height * 0.5);
    // SkPath path;
    // path.addArc(
    //     rect, shp->startAngle(), shp->startAngle() - shp->endAngle()
    // );
    // path.close();

    // SkMatrix matrix;

    // matrix.setRotate(-shp->rotation());
    // matrix.preTranslate(-center.x(), -center.y());
    // path.transform(matrix);
    // matrix.reset();
    // matrix.setTranslate(center.x(), center.y());
    // path.transform(matrix);

    // SkPaint paint;
    // paint = shp->fill()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);

    // paint = shp->stroke()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
}


void SymCanvas::draw(SymPie* shp) {
    SymPoint center = shp->center().transform(_transformMatrix);

    cairo_save(_canvas);
    cairo_translate(_canvas, center.x(), center.y());
    cairo_rotate(_canvas, -DEG_TO_RAD * shp->rotation());
    cairo_scale(_canvas, shp->radiusX() * _dotsPerMM * _width * 0.5, shp->radiusY() * _dotsPerMM * _height * 0.5);
    cairo_arc_negative(_canvas, 0.0, 0.0, 1.0, -shp->_startAngle * DEG_TO_RAD, -shp->_endAngle * DEG_TO_RAD);
    cairo_line_to(_canvas, 0, 0);
    cairo_close_path(_canvas);
    cairo_restore(_canvas);

    setFillStyle(shp);
    cairo_fill_preserve(_canvas);
    setStrokeStyle(shp);
    cairo_stroke(_canvas);

    // SkRect rect = SkRect::MakeLTRB(
    //     center.x() - shp->radiusX() * _dotsPerMM * _width * 0.5,
    //     center.y() - shp->radiusY() * _dotsPerMM * _height * 0.5,
    //     center.x() + shp->radiusX() * _dotsPerMM * _width * 0.5,
    //     center.y() + shp->radiusY() * _dotsPerMM * _height * 0.5);
    // SkPath path;
    // path.addArc(
    //     rect, shp->startAngle(), shp->startAngle() - shp->endAngle()
    // );
    // path.lineTo(center.x(), center.y());
    // path.close();

    // SkMatrix matrix;

    // matrix.setRotate(-shp->rotation());
    // matrix.preTranslate(-center.x(), -center.y());
    // path.transform(matrix);
    // matrix.reset();
    // matrix.setTranslate(center.x(), center.y());
    // path.transform(matrix);

    // SkPaint paint;
    // paint = shp->fill()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);

    // paint = shp->stroke()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
}

void SymCanvas::draw(SymSystemLine* shp) {
    SymPoint l = SymPoint(-1, 0).transform(_transformMatrix);
    SymPoint r = SymPoint(1, 0).transform(_transformMatrix);
    cairo_save(_canvas);
    cairo_move_to(_canvas, l.x(), l.y());
    cairo_line_to(_canvas, r.x(), r.y());
    cairo_restore(_canvas);
    setStrokeStyle(shp);
    cairo_stroke(_canvas);
    // SkPath path;
    // path.moveTo(l.x(), l.y());
    // path.lineTo(r.x(), r.y());
    // SkPaint paint = shp->stroke()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
}


void SymCanvas::draw(SymRegularPolygon* shp) {
    SymPoint center = shp->center().transform(_transformMatrix);
    std::vector<SymPoint> points = shp->getPoints();

    cairo_save(_canvas);
    cairo_translate(_canvas, center.x(), center.y());
    cairo_rotate(_canvas, -DEG_TO_RAD * shp->rotation());
    cairo_scale(_canvas, _dotsPerMM * _width * 0.5, -_dotsPerMM * _height * 0.5);
    cairo_move_to(_canvas, points[0].x(), points[0].y());
    for (int i = 1; i < points.size(); ++i) {
        cairo_line_to(_canvas, points[i].x(), points[i].y());
    }

    cairo_close_path(_canvas);
    cairo_restore(_canvas);

    setFillStyle(shp);
    cairo_fill_preserve(_canvas);
    setStrokeStyle(shp);
    cairo_stroke(_canvas);



    // SkPath path;
    // const std::vector<SymPoint>& points = shp->getPoints();
    // std::vector<SymPoint> transformedPoints;
    // for (auto& pt : points) {
    //     transformedPoints.push_back(pt.transform(_transformMatrix));
    // }
    // if (transformedPoints.size() < 2) return;

    // path.moveTo(transformedPoints[0].x(), transformedPoints[0].y());
    // for (int i = 1; i < transformedPoints.size(); ++i) {
    //     path.lineTo(transformedPoints[i].x(), transformedPoints[i].y());
    // }
    // path.close();

    // SkMatrix matrix;

    // matrix.setRotate(-shp->rotation());
    // matrix.preTranslate(-center.x(), -center.y());
    // path.transform(matrix);
    // matrix.reset();
    // matrix.setTranslate(center.x(), center.y());
    // path.transform(matrix);

    // SkPaint paint;
    // paint = shp->fill()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
    // paint = shp->stroke()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
}


void SymCanvas::draw(SymStar* shp) {

    SymPoint center = shp->center().transform(_transformMatrix);
    std::vector<SymPoint> points = shp->getPoints();

    cairo_save(_canvas);
    cairo_translate(_canvas, center.x(), center.y());
    cairo_rotate(_canvas, -DEG_TO_RAD * shp->rotation());
    cairo_scale(_canvas, _dotsPerMM * _width * 0.5, -_dotsPerMM * _height * 0.5);
    cairo_move_to(_canvas, points[0].x(), points[0].y());
    for (int i = 1; i < points.size(); ++i) {
        cairo_line_to(_canvas, points[i].x(), points[i].y());
    }

    cairo_close_path(_canvas);
    cairo_restore(_canvas);

    setFillStyle(shp);
    cairo_fill_preserve(_canvas);
    setStrokeStyle(shp);
    cairo_stroke(_canvas);
    // SkPath path;
    // const std::vector<SymPoint>& points = shp->getPoints();

    // std::vector<SymPoint> transformedPoints;
    // for (auto& pt : points) {
    //     transformedPoints.push_back(pt.transform(_transformMatrix));
    // }
    // if (transformedPoints.size() < 2) return;

    // path.moveTo(transformedPoints[0].x(), transformedPoints[0].y());
    // for (int i = 1; i < transformedPoints.size(); ++i) {
    //     path.lineTo(transformedPoints[i].x(), transformedPoints[i].y());
    // }
    // path.close();

    // SkMatrix matrix;

    // matrix.setRotate(-shp->rotation());
    // matrix.preTranslate(-center.x(), -center.y());
    // path.transform(matrix);
    // matrix.reset();
    // matrix.setTranslate(center.x(), center.y());
    // path.transform(matrix);

    // SkPaint paint;
    // paint = shp->fill()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
    // paint = shp->stroke()->toPaint(_dotsPerMM);
    // _canvas->drawPath(path, paint);
}


void SymCanvas::draw(SymText* shp) {


    cairo_save(_canvas);


    cairo_font_slant_t slant = CAIRO_FONT_SLANT_NORMAL;
    if (shp->_slant == SymText::SLANT_ITALIC) {

        slant = CAIRO_FONT_SLANT_ITALIC;
    }
    else if (shp->_slant == SymText::SLANT_OBLIQUE) {
        slant = CAIRO_FONT_SLANT_OBLIQUE;
    }
    else {
        slant = CAIRO_FONT_SLANT_NORMAL;
    }

    cairo_font_weight_t weight = CAIRO_FONT_WEIGHT_NORMAL;
    if (shp->_weight == SymText::WEIGHT_BOLD) {
        weight = CAIRO_FONT_WEIGHT_BOLD;
    }
    else {
        weight = CAIRO_FONT_WEIGHT_NORMAL;
    }

    cairo_select_font_face(_canvas, shp->_fontName.c_str(), slant, weight);
    cairo_set_font_size(_canvas, shp->_fontSize * 2.0 * _dotsPerMM * 0.5 * _width);
    cairo_save(_canvas);
    cairo_text_extents_t extents;
    cairo_text_extents(_canvas, shp->_text.c_str(), &extents);
    double anchorX = extents.x_bearing + extents.width * 0.5;
    double anchorY = extents.y_bearing + extents.height * 0.5;

    cairo_new_path(_canvas);  // 清除路径，以便下一次使用
    cairo_restore(_canvas);

    SymPoint pt = shp->_center.transform(_transformMatrix);
    cairo_translate(_canvas, pt.x(), pt.y());
    cairo_rotate(_canvas, -DEG_TO_RAD * shp->_rotation);
    cairo_move_to(_canvas, -anchorX, -anchorY);
    cairo_text_path(_canvas, shp->_text.c_str());
    cairo_restore(_canvas);

    setFillStyle(shp);
    cairo_fill_preserve(_canvas);
    setStrokeStyle(shp);
    cairo_stroke(_canvas);

}

void SymCanvas::begin() {
    if (_canvas) {
        cairo_destroy(_canvas);
        _canvas = NULL;
    }

    if (_surface) {
        cairo_surface_destroy(_surface);
        _surface = NULL;
    }

    _surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, _width * _dotsPerMM, _height * _dotsPerMM);
    _canvas = cairo_create(_surface);

    cairo_set_operator(_canvas, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgba(_canvas, 0, 0, 0, 0);  // 全透明
    cairo_paint(_canvas);
    cairo_set_operator(_canvas, CAIRO_OPERATOR_OVER);
}

void SymCanvas::end() {
    if (_surface) {
        cairo_surface_flush(_surface);
    }
}



typedef struct {
    char* data;
    size_t size;
}PngBuffer;

static cairo_status_t
my_png_save(void* closure,
    const unsigned char* data,
    unsigned int length) {
    PngBuffer* buffer = (PngBuffer*)closure;

    // 重新分配内存以容纳新数据
    char* new_data = (char*)realloc(buffer->data, buffer->size + length);
    if (!new_data) {
        return CAIRO_STATUS_NO_MEMORY;
    }

    buffer->data = new_data;
    memcpy(buffer->data + buffer->size, data, length);
    buffer->size += length;

    return CAIRO_STATUS_SUCCESS;
}

/**
 * 获取符号的图像数据。
 * 返回的数据请务必使用free()释放内存，不要使用delete释放内存。
 *
 */
char* SymCanvas::data(size_t& size) {
    PngBuffer buffer = { NULL, 0 };

    cairo_surface_write_to_png_stream(_surface, my_png_save, &buffer);
    size = buffer.size;
    // std::vector<char> png_data;

    // // 将表面写入内存
    // cairo_status_t status = cairo_surface_write_to_png_stream(
    //     _surface,
    //     [](void* closure, const unsigned char* data, unsigned int length) -> cairo_status_t {
    //         auto& vec = *static_cast<std::vector<char>*>(closure);
    //         vec.insert(vec.end(), data, data + length);
    //         return CAIRO_STATUS_SUCCESS;
    //     },
    //     &png_data);

    return buffer.data;
}


