#include "SymCanvas.h"




SymCanvas::SymCanvas() {
    set(10, 10, 96.0 / 25.4);
}


SymCanvas::~SymCanvas() {

}


void SymCanvas::set(double width, double height, double dotsPerMM) {
    _width = width;
    _height = height;
    _dotsPerMM = dotsPerMM;
}

