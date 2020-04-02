/** @file pbcPlaybook.h
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

#ifndef PBCPLAYBOOK_H
#define PBCPLAYBOOK_H

#include "util/pbcSingleton.h"
#include "util/pbcDeclarations.h"
#include "models/pbcCategory.h"
#include <ostream>
#include <boost/serialization/map.hpp>
#include <boost/serialization/access.hpp>
#include <vector>
#include <string>
#include <list>

class PBCFormation;
typedef boost::shared_ptr<PBCFormation> PBCFormationSP;
class PBCRoute;
typedef boost::shared_ptr<PBCRoute> PBCRouteSP;

class PBCPlaybook {
friend class boost::serialization::access;
 private:
    std::string _builtWithPBCVersion;
    std::string _name;
    PBCModelMap<PBCFormationSP> _formations;
    PBCModelMap<PBCRouteSP> _routes;
    PBCModelMap<PBCCategorySP> _categories;
    PBCModelMap<PBCPlaySP> _plays;
    unsigned int _playerNumber;

    template<class Archive>
    void save(Archive& ar, const unsigned int version) const {  // NOLINT
        ar << _builtWithPBCVersion;
        ar << _name;
        ar << _playerNumber;
        ar << _formations;
        ar << _routes;
        ar << _plays;
        ar << _categories;
    }

    template<class Archive>
    void load(Archive& ar, const unsigned int version) {  // NOLINT
        ar >> _builtWithPBCVersion;
        ar >> _name;
        if (version >= 1) {
            ar >> _playerNumber;
        }
        ar >> _formations;
        ar >> _routes;
        ar >> _plays;
        ar >> _categories;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()


 public:
    PBCPlaybook();
    void resetToNewEmptyPlaybook(const std::string& name,
                                 const unsigned int playerNumber);
    void reloadDefaultFormations();
    void setName(const std::string& name);
    bool addFormation(PBCFormationSP formation, bool overwrite = false);
    bool addRoute(PBCRouteSP route, bool overwrite = false);
    bool addCategory(PBCCategorySP category, bool overwrite = false);
    bool addPlay(PBCPlaySP play, bool overwrite = false);
    void deleteFormation(const std::string& name);
    void deleteRoute(const std::string& name);
    void deleteCategory(const std::string& name);
    void deletePlay(const std::string& name);
    std::string builtWithPBCVersion();
    std::string name() const;
    std::list<PBCFormationSP> formations() const;
    std::list<PBCRouteSP> routes() const;
    std::list<PBCCategorySP> categories() const;
    std::list<PBCPlaySP> plays() const;
    bool hasFormation(const std::string& name);
    PBCFormationSP getFormation(const std::string& name);
    PBCPlaySP getPlay(const std::string& name);
    PBCRouteSP getRoute(const std::string& name);
    PBCCategorySP getCategory(const std::string& name);
    std::vector<std::string> getFormationNames() const;
    std::vector<std::string> getRouteNames() const;
    std::vector<std::string> getPlayNames() const;
    std::vector<std::string> getCategoryNames() const;
};
BOOST_CLASS_VERSION(PBCPlaybook, 1)

#endif  // PBCPLAYBOOK_H
