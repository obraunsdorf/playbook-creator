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


PBCRole PBCPlayer::role() const {
    return _role;
}

void PBCPlayer::setRole(const PBCRole &role) {
    _role = role;
}

PBCColor PBCPlayer::color() const {
    return _color;
}

void PBCPlayer::setColor(const PBCColor &color) {
    _color = color;
}

PBCDPoint PBCPlayer::pos() const {
    return _pos;
}

void PBCPlayer::setPos(const PBCDPoint &pos) {
    _pos = pos;
}

PBCRouteSP PBCPlayer::route() const {
    return _route;
}

void PBCPlayer::setRoute(const PBCRouteSP &route) {
    _route = route;
}

PBCMotionSP PBCPlayer::motion() const {
    return _motion;
}

void PBCPlayer::setMotion(const PBCMotionSP &motion) {
    _motion = motion;
}

PBCPlayer::PBCPlayer(PBCRole role,
                     PBCColor color,
                     PBCDPoint pos,
                     PBCRouteSP route,
                     PBCMotionSP motion) :
    _role(role),
    _color(color),
    _pos(pos),
    _route(route),
    _motion(motion) {}


