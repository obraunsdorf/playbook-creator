/** @file pbcPlaybook.cpp
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

#include "pbcPlaybook.h"
#include <list>
#include <utility>
#include <string>
#include <vector>
#include "util/pbcExceptions.h"
#include "util/pbcStorage.h"

PBCPlaybook::PBCPlaybook() : _name("new Playbook") {
    resetToNewEmptyPlaybook(_name);
}

void PBCPlaybook::resetToNewEmptyPlaybook(const std::string &name) {
    _builtWithPBCVersion = PBCVersion::getVersionString();
    _name = name;
    _formations.clear();
    _routes.clear();
    _categories.clear();
    _plays.clear();

    std::vector<PBCPathSP> path_5In({ PBCPathSP(new PBCPath(0, 5)),
                                      PBCPathSP(new PBCPath(2, 5))});
    PBCRouteSP route_5In(new PBCRoute("5 In", "", path_5In));
    _routes.insert(std::make_pair(route_5In->name(), route_5In));


    std::vector<PBCPathSP> path_Post({ PBCPathSP(new PBCPath(0, 5)),
                                       PBCPathSP(new PBCPath(2, 7))});
    PBCRouteSP route_Post(new PBCRoute("Post", "", path_Post));
    _routes.insert(std::make_pair(route_Post->name(), route_Post));


    std::vector<PBCPathSP> path_Slant({ PBCPathSP(new PBCPath(0, 2)),
                                        PBCPathSP(new PBCPath(2, 4))});
    PBCRouteSP route_Slant(new PBCRoute("Slant", "", path_Slant));
    _routes.insert(std::make_pair(route_Slant->name(), route_Slant));


    std::vector<PBCPathSP> path_Fly({ PBCPathSP(new PBCPath(0, 7))});
    PBCRouteSP route_Fly(new PBCRoute("Fly", "", path_Fly));
    _routes.insert(std::make_pair(route_Fly->name(), route_Fly));


    PBCFormationSP formation(new PBCFormation("PBC_StandardFormation"));
    formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Center", "C"},
                                                   PBCColor(),
                                                   PBCDPoint(0, 0))));

    formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Quarterback", "QB"},
                                                   PBCColor(),
                                                   PBCDPoint(0, -1))));

    formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Wide Receiver Left", "WRL"},  // NOLINT
                                                   PBCColor(),
                                                   PBCDPoint(-15, 0))));

    formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Wide Receiver Right", "WRR"}, // NOLINT
                                                   PBCColor(),
                                                   PBCDPoint(15, 0))));

    formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Runningback", "RB"},
                                                   PBCColor(),
                                                   PBCDPoint(0, -5))));

    std::pair<PBCModelMap<PBCFormationSP>::iterator, bool> result =
            _formations.insert(std::make_pair(formation->name(), formation));

    assert(result.second == true);
}

void PBCPlaybook::setName(const std::string &name) {
    _name = name;
}

bool PBCPlaybook::addFormation(PBCFormationSP formation, bool overwrite) {
    if(overwrite == true ) {
        PBCFormationSP formationCopy(new PBCFormation(*formation));
        _formations[formationCopy->name()] = formationCopy;
        PBCStorage::getInstance()->automaticSavePlaybook();
        return true;
    } else {
        PBCFormationSP formationCopy(new PBCFormation(*formation));
        InsertResult<PBCFormationSP> result =
                _formations.insert(std::make_pair(formation->name(),
                                                  formationCopy));
        if(result.second == true) {
            PBCStorage::getInstance()->automaticSavePlaybook();
        }
        return result.second;
    }
}

bool PBCPlaybook::addRoute(PBCRouteSP route, bool overwrite) {
    if(overwrite == true ) {
        *_routes[route->name()] = *route;  // TODO(obr): does this create memory leaks?  //NOLINT
        // _routes[route->name()] = route;  --> Routes in Plays are not changed when you overwrite them // NOLINT
        PBCStorage::getInstance()->automaticSavePlaybook();
        return true;
    } else {
        InsertResult<PBCRouteSP> result =
                _routes.insert(std::make_pair(route->name(), route));
        if(result.second == true) {
            PBCStorage::getInstance()->automaticSavePlaybook();
        }
        return result.second;
    }
}

bool PBCPlaybook::addCategory(PBCCategorySP category, bool overwrite) {
    if(overwrite == true) {
        _categories[category->name()] = category;
        PBCStorage::getInstance()->automaticSavePlaybook();
        return true;
    } else {
        InsertResult<PBCCategorySP> result =
                _categories.insert(std::make_pair(category->name(), category));
        if(result.second == true) {
            PBCStorage::getInstance()->automaticSavePlaybook();
        }
        return result.second;
    }
}

bool PBCPlaybook::addPlay(PBCPlaySP play, bool overwrite) {
    if(overwrite == true) {
        PBCPlaySP playCopy(new PBCPlay(*play));
        _plays[playCopy->name()] = playCopy;
        PBCStorage::getInstance()->automaticSavePlaybook();
        return true;
    } else {
        PBCPlaySP playCopy(new PBCPlay(*play));

        InsertResult<PBCPlaySP> result =
                _plays.insert(std::make_pair(play->name(), playCopy));
        if(result.second == true) {
            PBCStorage::getInstance()->automaticSavePlaybook();
        }
        return result.second;
    }
}

std::string PBCPlaybook::builtWithPBCVersion() {
    return _builtWithPBCVersion;
}

std::string PBCPlaybook::name() const {
    return _name;
}

std::list<PBCFormationSP> PBCPlaybook::formations() const {
    return mapToList<PBCFormationSP>(_formations);
}

std::list<PBCRouteSP> PBCPlaybook::routes() const {
    return mapToList<PBCRouteSP>(_routes);
}

std::list<PBCPlaySP> PBCPlaybook::plays() const {
    return mapToList<PBCPlaySP>(_plays);
}

PBCFormationSP PBCPlaybook::getFormation(const std::string &name) {
    const auto& it = _formations.find(name);
    assert(it != _formations.end());  // TODO(obr): throw exception
    PBCFormationSP formation = it->second;
    return PBCFormationSP(new PBCFormation(*formation));
}

PBCPlaySP PBCPlaybook::getPlay(const std::string &name) {
    const auto& it = _plays.find(name);
    assert(it != _plays.end());  // TODO(obr) throw exception
    PBCPlaySP play = it->second;
    return PBCPlaySP(new PBCPlay(*play));
}

std::vector<std::string> PBCPlaybook::getFormationNames() const {
    std::vector<std::string> formationNames;
    for(const auto& kv : _formations) {
        PBCFormationSP formation = kv.second;
        formationNames.push_back(formation->name());
    }
    return formationNames;
}

std::vector<std::string> PBCPlaybook::getPlayNames() const {
    std::vector<std::string> playNames;
    for(const auto& kv : _plays) {
        PBCPlaySP play = kv.second;
        playNames.push_back(play->name());
    }
    return playNames;
}
