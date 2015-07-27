/** @file pbcColor.cpp
    This file is part of Playbook Creator.

    Playbook Creator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Playbook Creator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Playbook Creator.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2015 Oliver Braunsdorf

    @author Oliver Braunsdorf
*/

#include "pbcColor.h"
#include <vector>

PBCColor::PBCColor() {
    _r = 0;
    _g = 0;
    _b = 0;
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

unsigned int PBCColor::r() const {
    return _r;
}

unsigned int PBCColor::g() const {
    return _g;
}

unsigned int PBCColor::b() const {
    return _b;
}
