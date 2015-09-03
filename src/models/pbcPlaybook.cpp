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

/**
 * @class PBCPlaybook
 * @brief This is a data model class representing a playbook.
 *
 * PBCPlaybook is a Singleton class (it inherits PBCSingleton). Thus the
 * application handles only one playbook at a time. It stores the plays,
 * formations, routes, and categories.
 */

/**
 * @brief The default constructor. It is only called by PBCSingleton and creates
 * a new empty playbook when the application is started.
 */
PBCPlaybook::PBCPlaybook() : _name("new Playbook") {
    resetToNewEmptyPlaybook(_name);
}


/**
 * @brief Resets the playbook if the user wants to create a new one.
 *
 * Standard Routes (5 In, Post, Fly, Slant) and a I-Formation are inserted.
 * @param name The name of the new Playbook
 */
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

/**
 * @brief Adds a copy of a formation to the playbook.
 * @param formation The formation to add.
 * @param overwrite Specifies if a formation with the same name should be overwritten.
 *
 * If overwrite is false and the formation has the same name
 * as a formation that has been added to the playbook already, then the new
 * formation is not added to the playbook. If overwrite is true, a formation
 * with the same name is overwritten by the new formation.
 * @return true if the formation has been successfully added to the playbook
 */
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

/**
 * @brief Adds a route to the playbook.
 *
 * Actually this function adds a pointer to a route. When overwriting a route
 * later, this pointer is used so that the routes which have the same name will
 * be overwritten in all plays. So you only have to change a route one time and
 * it is changed automatically in the whole playbook
 * @param route The route to add
 * @param overwrite Specifies if a route with the same name should be overwritten.
 *
 * If overwrite is false and the route has the same name as an formation that
 * has been added to the playbook already, then the new route is not added to
 * the playbook. If overwrite is true, a route with the same name is overwritten
 * by the new route.
 * @return true if the route has been successfully added to the playbook
 */
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

/**
 * @brief Adds a category to the playbook.
 * @param category The category to add.
 * @param overwrite Specifies if a formation with the same name should be overwritten.
 *
 * If overwrite is false and the category has the same name
 * as a category that has been added to the playbook already, then the new
 * category is not added to the playbook. If overwrite is true, a category
 * with the same name is overwritten by the new category.
 * @return true if the category has been successfully added to the playbook
 */
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

/**
 * @brief Adds a copy of a play to the playbook.
 * @param play The formation to add.
 * @param overwrite Specifies if a play with the same name should be overwritten.
 *
 * If overwrite is false and the play has the same name
 * as a play that has been added to the playbook already, then the new
 * play is not added to the playbook. If overwrite is true, a play
 * with the same name is overwritten by the new play.
 * @return true if the play has been successfully added to the playbook
 */
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

/**
 * @brief Getter function to get the version of the application that this
 * playbook was created with.
 * @return the version string
 */
std::string PBCPlaybook::builtWithPBCVersion() {
    return _builtWithPBCVersion;
}

/**
 * @brief Setter function for the playbook's name
 * @param name The new name of the playbook
 */
void PBCPlaybook::setName(const std::string &name) {
    _name = name;
}

/**
 * @brief Getter function for the playbook's name
 * @return The playbook's name
 */
std::string PBCPlaybook::name() const {
    return _name;
}

/**
 * @brief Getter function for the formations of the playbook
 * @return A list of the playbook's formations
 */
std::list<PBCFormationSP> PBCPlaybook::formations() const {
    return mapToList<PBCFormationSP>(_formations);
}

/**
 * @brief Getter function for the routes of the playbook
 * @return A list of the playbook's routes
 */
std::list<PBCRouteSP> PBCPlaybook::routes() const {
    return mapToList<PBCRouteSP>(_routes);
}

/**
 * @brief Getter function for the plays of the playbook
 * @return A list of the playbook's plays
 */
std::list<PBCPlaySP> PBCPlaybook::plays() const {
    return mapToList<PBCPlaySP>(_plays);
}

/**
 * @brief Selects a formation by name
 * @param name The name of the formation
 * @return The formation with the given name. If no formation with the given
 * name is in the playbook, an exception is thrown (should be ;) )
 */
PBCFormationSP PBCPlaybook::getFormation(const std::string &name) {
    const auto& it = _formations.find(name);
    assert(it != _formations.end());  // TODO(obr): throw exception
    PBCFormationSP formation = it->second;
    return PBCFormationSP(new PBCFormation(*formation));
}

/**
 * @brief Selects a play by name
 * @param name The name of the play
 * @return The play with the given name. If no play with the given
 * name is in the playbook, an exception is thrown (should be ;) )
 */
PBCPlaySP PBCPlaybook::getPlay(const std::string &name) {
    const auto& it = _plays.find(name);
    assert(it != _plays.end());  // TODO(obr) throw exception
    PBCPlaySP play = it->second;
    return PBCPlaySP(new PBCPlay(*play));
}

/**
 * @brief Gets the names of the playbook's formations
 * @return a list of formation names
 */
std::vector<std::string> PBCPlaybook::getFormationNames() const {
    std::vector<std::string> formationNames;
    for(const auto& kv : _formations) {
        PBCFormationSP formation = kv.second;
        formationNames.push_back(formation->name());
    }
    return formationNames;
}

/**
 * @brief Gets the names of the playbook's play
 * @return a list of play names
 */
std::vector<std::string> PBCPlaybook::getPlayNames() const {
    std::vector<std::string> playNames;
    for(const auto& kv : _plays) {
        PBCPlaySP play = kv.second;
        playNames.push_back(play->name());
    }
    return playNames;
}
