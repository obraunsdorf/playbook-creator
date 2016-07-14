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
#include <list>
#include "models/pbcCategory.h"
#include <ostream>
#include <boost/serialization/map.hpp>
#include <boost/serialization/access.hpp>
#include <map>
#include <string>
#include <utility>
#include <vector>

class PBCFormation;
typedef boost::shared_ptr<PBCFormation> PBCFormationSP;
class PBCRoute;
typedef boost::shared_ptr<PBCRoute> PBCRouteSP;

class PBCPlaybook : public PBCSingleton<PBCPlaybook> {
friend class PBCSingleton<PBCPlaybook>;
friend class boost::serialization::access;
 private:
    template<typename T> using PBCModelMap = std::map<std::string, T>;

    std::string _builtWithPBCVersion;
    std::string _name;
    PBCModelMap<PBCFormationSP> _formations;
    PBCModelMap<PBCRouteSP> _routes;
    PBCModelMap<PBCCategorySP> _categories;
    PBCModelMap<PBCPlaySP> _plays;


    template<typename T>
    using InsertResult = std::pair<typename PBCModelMap<T>::iterator, bool>;

    template<class T>
    std::list<T> mapToList(PBCModelMap<T> map) const {
        std::list<T> list;
        for(const auto& kv : map) {
            list.push_back(kv.second);
        }
        return list;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {  // NOLINT
        assert(version == 0);
        ar & _builtWithPBCVersion;
        ar & _name;
        ar & _formations;
        ar & _routes;
        ar & _plays;
        ar & _categories;
    }

 protected:
    PBCPlaybook();

 public:
    void resetToNewEmptyPlaybook(const std::string& name,
                                 const int playerNumber);
    void setName(const std::string& name);
    bool addFormation(PBCFormationSP formation, bool overwrite = false);
    bool addRoute(PBCRouteSP route, bool overwrite = false);
    bool addCategory(PBCCategorySP category, bool overwrite = false);
    bool addPlay(PBCPlaySP play, bool overwrite = false);
    std::string builtWithPBCVersion();
    std::string name() const;
    std::list<PBCFormationSP> formations() const;
    std::list<PBCRouteSP> routes() const;
    std::list<PBCPlaySP> plays() const;
    PBCFormationSP getFormation(const std::string& name);
    PBCPlaySP getPlay(const std::string& name);
    std::vector<std::string> getFormationNames() const;
    std::vector<std::string> getPlayNames() const;
};

#endif  // PBCPLAYBOOK_H
