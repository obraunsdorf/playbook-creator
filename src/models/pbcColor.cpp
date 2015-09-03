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

/**
 * @class PBCColor
 * @brief This class stores color information.
 *
 * PBCColor is a data model class which stores color data as RGB values.
 */

/**
 * @brief The default constructor. Creates a black color.
 */
PBCColor::PBCColor() {
    _r = 0;
    _g = 0;
    _b = 0;
}

/**
 * @brief The constructor. Creates the color specified by the RGB parameters
 * @param r The red color component
 * @param g The green color component
 * @param b The blue color component
 */
PBCColor::PBCColor(unsigned int r, unsigned int g, unsigned int b) {
    assert(r <= 255);
    assert(g <= 255);
    assert(b <= 255);
    _r = r;
    _g = g;
    _b = b;
}

/**
 * @brief The copy constructor.
 * @param obj Another color object
 */
PBCColor::PBCColor(const PBCColor &obj) {
    _r = obj._r;
    _g = obj._g;
    _b = obj._b;
}

/**
 * @brief Getter function for the red color component
 * @return the red color component
 */
unsigned int PBCColor::r() const {
    return _r;
}

/**
 * @brief Getter function for the green color component
 * @return the green color component
 */
unsigned int PBCColor::g() const {
    return _g;
}

/**
 * @brief Getter function for the blue color component
 * @return the blue color component
 */
unsigned int PBCColor::b() const {
    return _b;
}
