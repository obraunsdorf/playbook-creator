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
#include <list>


std::string PBCPlay::name() const {
    return _name;
}

void PBCPlay::setName(const std::string &name) {
    _name = name;
}

std::string PBCPlay::codeName() const {
    return _codeName;
}

void PBCPlay::setCodeName(const std::string &codeName) {
    _codeName = codeName;
}

PBCFormationSP PBCPlay::formation() const {
    return _formation;
}

void PBCPlay::setFormation(const PBCFormationSP &formation) {
    _formation = formation;
}

std::list<PBCCategorySP> PBCPlay::categories() const {
    return _categories;
}

void PBCPlay::setCategories(const std::list<PBCCategorySP> &categories) {
    _categories = categories;
}

PBCPlay::PBCPlay(const std::string &name,
                 const std::string &codeName,
                 const std::string& formationName) :
    _name(name),
    _codeName(codeName),
    _formation(PBCPlaybook::getInstance()->getFormation(formationName)) {}

PBCPlay::PBCPlay(const PBCPlay &other) :
    _name(other.name()),
    _codeName(other.codeName()),
    _formation(new PBCFormation(*other.formation())),
    _categories(other.categories()) {
    PBCFormation::iterator it = _formation->begin();
    PBCFormation::iterator otherIt = other.formation()->begin();
    while(it != _formation->end() && otherIt != other.formation()->end()) {
        PBCPlayerSP playerSP = *it;
        PBCPlayerSP otherPlayerSP = *otherIt;
        playerSP->setRoute(otherPlayerSP->route());
        playerSP->setMotion(otherPlayerSP->motion());
        ++it;
        ++otherIt;
    }
    assert(it == _formation->end() && otherIt == other.formation()->end());
}
