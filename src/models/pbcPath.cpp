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

/**
 * @class PBCPath
 * @brief This is a data model class that represents a path.
 *
 * A path is a single straight or curved line to a specified endpoint in Yards.
 * Routes and motions consist of multiple paths. There is no startpoint
 * specified for a path. In a route, for example, the startpoint of a path is
 * always the endpoint of the previous path and the startpoint of the first path
 * is the players position.
 */

/**
 * @brief A getter function for the path's endpoint.
 * @return The path's endpoint in yd
 */
PBCDPoint PBCPath::endpoint() const {
    return _endpoint;
}

/**
 * @brief Determines whether the path is straight or an arc
 * @return true if the path is an arc, false if the path is straight
 */
bool PBCPath::isArc() const {
    return _arc;
}

/**
 * @brief Determines whether the arc path is concave or convex
 * @return true if the path is concave, false if the path is convex
 * @see https://en.wikipedia.org/wiki/Convex_function
 */
bool PBCPath::isConcave() const {
    return _concave;
}

/**
 * @brief The constructor. Creates a path to the given endpoint
 * @param endpoint Specifies the endpoint (in yd)
 * @param arc true if the path should be an arc, false if it should be straigt
 * @param concave true if the arc path should be concave, false if it should be convex
 */
PBCPath::PBCPath(PBCDPoint endpoint, bool arc, bool concave) :
    _endpoint(endpoint),
    _arc(arc),
    _concave(concave) {}

/**
 * @brief A convenient constructor, in which the endpoint is created by x,y-Coodinates.
 * @param endpointX The horizontal coordinate (in yd)
 * @param endpointY The vertical coordinate (in yd)
 * @param arc true if the path should be an arc, false if it should be straigt
 * @param concave concave true if the arc path should be concave, false if it should be convex
 */
PBCPath::PBCPath(double endpointX, double endpointY, bool arc, bool concave) :
    _endpoint(PBCDPoint(endpointX, endpointY)),
    _arc(arc),
    _concave(concave) {}
