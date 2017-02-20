/** @file pbcCategory.cpp
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

    Copyright 2016 Oliver Braunsdorf

    @author Oliver Braunsdorf
*/

#include "pbcCategory.h"
#include "pbcPlay.h"
#include <set>
#include <string>

/**
 * @class PBCCategory.cpp
 * @brief A model class which represents a category of plays.
 *
 * A category has a member set of PBCPlay instances which belong to this particular category.
 * In addition every PBCPlay has a set of categories it belongs to. These two sets shall provide
 * bidirectional access and must be consistent always.
 */

/**
 * @brief Getter function for the name of the category
 * @return the name of the category
 */
std::string PBCCategory::name() const {
    return _name;
}


/**
 * @brief Setter function for the name of the category
 * @param name the new name of the category
 */
void PBCCategory::setName(const std::string &name) {
    _name = name;
}

/**
 * @brief Inserts the given play into this category
 * @param play which is added to this category
 */
void PBCCategory::addPlay(const PBCPlaySP &play) {
    _plays.insert(play);
}

/**
 * @brief Removes the given play from this category
 * @param play which is removed from this category
 */
void PBCCategory::removePlay(const PBCPlaySP &play) {
    _plays.erase(play);
}

/**
 * @brief Getter for the set of plays of this category
 * @return the set of plays
 */
std::set<PBCPlaySP> PBCCategory::plays() const {
    return _plays;
}
