/** @file pbcPath.cpp
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

#include "pbcPath.h"


PBCDPoint PBCPath::endpoint() const {
    return _endpoint;
}

bool PBCPath::isArc() const {
    return _arc;
}

bool PBCPath::isConcave() const {
    return _concave;
}

PBCPath::PBCPath(PBCDPoint endpoint, bool arc, bool concave) :
    _endpoint(endpoint),
    _arc(arc),
    _concave(concave) {}

PBCPath::PBCPath(double endpointX, double endpointY, bool arc, bool concave) :
    _endpoint(PBCDPoint(endpointX, endpointY)),
    _arc(arc),
    _concave(concave) {}
