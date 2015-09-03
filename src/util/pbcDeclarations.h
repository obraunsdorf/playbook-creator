/** @file pbcDeclarations.h
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

#ifndef PBCDECLARATIONS_H
#define PBCDECLARATIONS_H

#include <boost/shared_ptr.hpp>
#include <boost/geometry.hpp>
#include <boost/array.hpp>

using boost::geometry::model::point;
/**
 * @brief PBCDPoint is a 2D point consisting of two coordinates of type double
 */
typedef point<double, 2, boost::geometry::cs::cartesian> PBCDPoint;


#endif  // PBCDECLARATIONS_H
