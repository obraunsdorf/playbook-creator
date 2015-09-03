/** @file pbcMotion.cpp
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

#include "pbcMotion.h"
#include "util/pbcExceptions.h"

/**
 * @class PBCMotion
 * @brief This is a data model class, which represents a players motion before
 * the snap.
 *
 * It is inherited from PBCAbstractMovement.
 */

/**
 * @brief The default constructor. Creates an empty motion.
 */
PBCMotion::PBCMotion() : _motionEndPoint(0, 0) {
}

/**
 * @brief Adds a path to the motion.
 * @param pathSP The path to add
 */
void PBCMotion::addPath(const PBCPathSP& pathSP) {
    if(pathSP->endpoint().get<1>() > _motionEndPoint.get<1>()) {
        throw PBCRuleBreakException("A motion cannot go towards the LOS.");
    }
    PBCAbstractMovement::addPath(pathSP);
    _motionEndPoint = pathSP->endpoint();
}

/**
 * @brief A getter Function for the motion's endpoint
 *
 * The motion's endpoint is the position (in yd) where the player will stand
 * when he has finished his motion
 * @return The motion endpoint
 */
PBCDPoint PBCMotion::motionEndPoint() const {
    return _motionEndPoint;
}
