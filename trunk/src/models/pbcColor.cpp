#include "pbcColor.h"

PBCColor::PBCColor()
{
}


PBCColor::PBCColor(unsigned int r, unsigned int g, unsigned int b) {
    assert(r <= 255);
    assert(g <= 255);
    assert(b <= 255);
    _r = r;
    _g = g;
    _b = b;
}


PBCColor::PBCColor(const PBCColor &obj) {
    _r = obj._r;
    _g = obj._g;
    _b = obj._b;
}


std::vector<unsigned int> PBCColor::getColorVector() const {
    std::vector<unsigned int> color;
    color.push_back(_r);
    color.push_back(_g);
    color.push_back(_b);
    return color;
}

unsigned int PBCColor::r() const {
    return _r;
}

unsigned int PBCColor::g() const {
    return _g;
}

unsigned int PBCColor::b() const {
    return _b;
}
