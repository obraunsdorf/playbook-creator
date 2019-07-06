/** @file pbcPositionTranslator.cpp
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

#include "pbcPositionTranslator.h"

/**
 * @brief Calculates the middle of the field (grid iron), which is also the
 * ball's location.
 * @return the ball's location
 */
PBCDPoint PBCPositionTranslator::ballPos() {
    unsigned int y = PBCConfig::getInstance()->losY();
    unsigned int x = PBCConfig::getInstance()->canvasWidth() / 2;
    return PBCDPoint(x, y);
}

/**
 * @brief Translates a position given in yards into pixel coordinates
 * @param pos The position in yards to translate
 * @param center The point (in pixels) to which pos is relative to
 * @return the pixel coordinates of the given position
 */
PBCDPoint PBCPositionTranslator::translatePos(PBCDPoint pos, PBCDPoint center) {
    unsigned int factor = PBCConfig::getInstance()->ydInPixel();
    pbcAssert(factor > 0);
    return PBCDPoint(center.get<0>() + factor * pos.get<0>(),
                     center.get<1>() - factor * pos.get<1>());
}


/**
 * @brief Translates a position given in pixel coordinates into pixel yards
 *
 * This is the inverse function of PBCPositionTranslator::translatePos()
 * @param pos The position in pixel coordinates to translate
 * @param center The point (in pixels) to which pos is relative to
 * @return the coordinates in yard of the given position
 */
PBCDPoint PBCPositionTranslator::retranslatePos(PBCDPoint pos, PBCDPoint center) { // NOLINT
    // TODO(obr): refactor this function: ydInPixel is already a double
    unsigned int factor = PBCConfig::getInstance()->ydInPixel();
    pbcAssert(factor > 0);
    double x = (pos.get<0>() - center.get<0>()) / static_cast<double>(factor);
    double y = (pos.get<1>() - center.get<1>()) / static_cast<double>(factor) * (-1); // NOLINT
    return PBCDPoint(x, y);
}
