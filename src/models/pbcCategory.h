/** @file pbcCategory.h
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

#ifndef PBCCATEGORY_H
#define PBCCATEGORY_H

#include "util/pbcDeclarations.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/set.hpp>
#include <assert.h>
#include <list>
#include <set>

class PBCPlay;
typedef boost::shared_ptr<PBCPlay> PBCPlaySP;

class PBCCategory;
typedef boost::shared_ptr<PBCCategory> PBCCategorySP;

class PBCCategory{
friend class boost::serialization::access;

 private:
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {  // NOLINT
        assert(version == 0);
        ar & _name;
        ar & _plays;
    }
    PBCCategory() {}

 protected:
    std::string _name;
    // char _shortName[5];
    std::set<PBCPlaySP> _plays;

 public:
    explicit PBCCategory(const std::string& name) : _name(name) {}
    std::string name() const;
    void setName(const std::string& name);
    void addPlay(const PBCPlaySP& play);
    void removePlay(const PBCPlaySP& play);
    std::set<PBCPlaySP> plays() const;
};

#endif  // PBCCATEGORY_H
