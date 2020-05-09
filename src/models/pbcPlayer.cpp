/** @file pbcPlayer.cpp
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

#include "pbcPlayer.h"

/**
 * @class PBCPlayer
 * @brief A model class that stores information about a player
 */

/**
 * @brief Getter for the player's role
 *
 * Here a role is a synonym for the players position (Quarterback, Runningback, Center)
 * @return the player's role
 */
PBCRole PBCPlayer::role() const {
    return _role;
}


/**
 * @brief Setter for the player's role
 *
 * Here a role is a synonym for the players position (Quarterback, Runningback, Center)
 * @param role The player's role
 */
void PBCPlayer::setRole(const PBCRole &role) {
    _role = role;
}

/**
 * @brief Getter for the player's color
 *
 * This color is used for painting the player's body, route and motion.
 * @return the player's color
 */
PBCColor PBCPlayer::color() const {
    return _color;
}

/**
 * @brief Setter for the player's color
 *
 * This color is used for painting the player's body, route and motion.
 * @param color The new color of the player
 */
void PBCPlayer::setColor(const PBCColor &color) {
    _color = color;
}

/**
 * @brief Getter for the (geographical) position of the player (in yd)
 * @return the player's position
 */
PBCDPoint PBCPlayer::pos() const {
    return _pos;
}

/**
 * @brief @brief Setter for the (geographical) position of the player (in yd)
 * @param pos The player's new position
 */
void PBCPlayer::setPos(const PBCDPoint &pos) {
    _pos = pos;
}

/**
 * @brief Getter for the route of the player
 * @return the player's route
 */
PBCRouteSP PBCPlayer::route() const {
    return _route;
}

/**
 * @brief Setter for the route of the player
 * @param route The player's new route
 */
void PBCPlayer::setRoute(const PBCRouteSP &route) {
    _route = route;
}

/**
 * @brief Getter for the motion of the player
 * @return the player's motion
 */
PBCMotionSP PBCPlayer::motion() const {
    return _motion;
}

/**
 * @brief Setter for the motion of the player
 * @param motion The player's new motion
 */
void PBCPlayer::setMotion(const PBCMotionSP &motion) {
    _motion = motion;
}

/**
 * @brief The constructor
 * @param role The role of the created player
 * @param color The color of the created player
 * @param pos The position of the created player
 * @param route The route of the created player
 * @param motion The motion of the created player
 */
PBCPlayer::PBCPlayer(PBCRole role,
                     PBCColor color,
                     PBCDPoint pos,
                     const std::string& name,
                     unsigned int nr,
                     PBCRouteSP route,
                     PBCMotionSP motion) :
    _role(role),
    _color(color),
    _pos(pos),
    _name(name),
    _nr(nr),
    _route(route),
    _motion(motion){}

const std::string &PBCPlayer::name() const {
    return _name;
}

void PBCPlayer::setName(const std::string &name) {
    _name = name;
}

unsigned int PBCPlayer::nr() const {
    return _nr;
}

void PBCPlayer::setNr(unsigned int nr) {
    _nr = nr;
}


