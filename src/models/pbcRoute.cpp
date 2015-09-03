/** @file pbcRoute.cpp
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

#include "pbcRoute.h"
#include <string>
#include <vector>

/**
 * @class PBCRoute
 * @brief This is a data model class, which represents a players route.
 *
 * It is inherited from PBCAbstractMovement.
 */

/**
 * @brief The constructor
 * @param name The name of the created route
 * @param codeName The code name of the created route
 * @param paths The paths that the created route consists of
 */
PBCRoute::PBCRoute(const std::string &name,
                   const std::string &codeName,
                   const std::vector<PBCPathSP>& paths) :
    PBCAbstractMovement(paths),
    _name(name),
    _codeName(codeName) {}

/**
 * @brief Getter for the name of the route
 * @return the route's name
 */
std::string PBCRoute::name() const {
    return _name;
}

/**
 * @brief Getter for the code name of the route
 * @return the route's code name
 */
std::string PBCRoute::codeName() const {
    return _codeName;
}
