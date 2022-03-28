/** @file pbcPlay.cpp
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

#include "pbcPlay.h"
#include <string>
#include <set>
#include <list>

/**
 * @class PBCPlay
 * @brief A model class that stores information about a play
 */

/**
 * @brief Getter function for the name of the play
 * @return the name of the play
 */
std::string PBCPlay::name() const {
    return _name;
}

/**
 * @brief Setter function for the name of the play
 * @param name the new name of the play
 */
void PBCPlay::setName(const std::string &name) {
    _name = name;
}

/**
 * @brief Getter function for the code name of the play
 * @return the code name of the play
 */
std::string PBCPlay::codeName() const {
    return _codeName;
}

/**
 * @brief Setter function for the code name of the play
 * @param codeName the new code name of the play
 */
void PBCPlay::setCodeName(const std::string &codeName) {
    _codeName = codeName;
}


/**
 * @brief Getter function for the formation of the play
 * @return the formation of the play
 */
PBCFormationSP PBCPlay::formation() const {
    return _formation;
}

/**
 * @brief Setter function for the formation of the play
 * @param formation the new formation of the play
 */
void PBCPlay::setFormation(const PBCFormationSP &formation) {
    _formation = formation;
}


/**
 * @brief Getter function for the categories that the play belongs to
 * @return a set of the categories
 */
std::set<PBCCategorySP> PBCPlay::categories() const {
    return _categories;
}


/**
 * @brief Adds a category to the set of categories that the play belongs to
 * @param category to add
 */
void PBCPlay::addCategory(const PBCCategorySP &category) {
    _categories.insert(category);
}


/**
 * @brief Removes a category from the set of categories that the play belongs to
 * @param category to add
 */
void PBCPlay::removeCategory(const PBCCategorySP &category) {
    _categories.erase(category);
}


/**
 * @brief The constructor
 * @param name the name of the created play
 * @param codeName the code name of the created play
 * @param formationName the formation name of the play
 *
 * The formation is fetched from the playbook by its name
 */
PBCPlay::PBCPlay(const std::string &name,
                 const std::string &codeName,
                 const std::string& formationName,
                 const std::string& comment) :
    _name(name),
    _codeName(codeName),
    _comment(comment) {
    if (PBCController::getInstance()->getPlaybook()->hasFormation(formationName)) {
        _formation = PBCController::getInstance()->getPlaybook()->getFormation(formationName);
    } else {
        _formation.reset(new PBCFormation(formationName));
    }
}

/**
 * @brief The copy constructor
 *
 * The name, code name and categories of the play are simply copied. The
 * formation needs special treatment because the copy constructor of
 * PBCFormation does not copy the players' routes and motions but only their
 * positions. So we need to set the routes and motions here manually.
 * @param other the PBCPlay instance to copy
 */
PBCPlay::PBCPlay(const PBCPlay &other) :
    _name(other.name()),
    _codeName(other.codeName()),
    _formation(new PBCFormation(*other.formation())),
    _categories(other.categories()),
    _comment(other.comment()){
    PBCFormation::iterator it = _formation->begin();
    PBCFormation::iterator otherIt = other.formation()->begin();
    while(it != _formation->end() && otherIt != other.formation()->end()) {
        PBCPlayerSP playerSP = *it;
        PBCPlayerSP otherPlayerSP = *otherIt;
        playerSP->setRoute(otherPlayerSP->route());
        playerSP->setMotion(otherPlayerSP->motion());
        playerSP->setAlternativeRoute(1, otherPlayerSP->alternativeRoute(1));
        playerSP->setAlternativeRoute(2, otherPlayerSP->alternativeRoute(2));
        for (PBCRouteSP optionRoute : otherPlayerSP->optionRoutes()) {
            playerSP->addOptionRoute(optionRoute);
        }
        ++it;
        ++otherIt;
    }
    pbcAssert(it == _formation->end() && otherIt == other.formation()->end());
}

std::string PBCPlay::comment() const {
    return _comment;
}

void PBCPlay::setComment(const std::string &comment) {
    _comment = comment;
}