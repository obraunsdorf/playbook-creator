/** @file pbcPlay.h
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

#ifndef PBCPLAY_H
#define PBCPLAY_H

#include <string>
#include <vector>

#include "models/pbcPlaybook.h"
#include "util/pbcDeclarations.h"
#include "models/pbcPlayer.h"
#include "models/pbcCategory.h"
#include "models/pbcFormation.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/set.hpp>
#include <set>

class PBCPlay;
typedef boost::shared_ptr<PBCPlay> PBCPlaySP;

class PBCPlay {
friend class boost::serialization::access;
friend class PBCCategory;
 private:
    std::string _name;
    std::string _codeName;
    PBCFormationSP _formation;
    std::set<PBCCategorySP> _categories;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {  // NOLINT
        pbcAssert(version == 0);
        ar & _name;
        ar & _codeName;
        ar & _formation;
        ar & _categories;
    }

    PBCPlay() {
        PBCPlay("", "", "");
    }

 public:
    PBCPlay(const std::string& name,
            const std::string& codeName,
            const std::string& formationName);

    PBCPlay(const PBCPlay& other);
    std::string name() const;
    void setName(const std::string &name);
    std::string codeName() const;
    void setCodeName(const std::string &codeName);
    PBCFormationSP formation() const;
    void setFormation(const PBCFormationSP &formation);
    std::set<PBCCategorySP> categories() const;
    void addCategory(const PBCCategorySP &category);
    void removeCategory(const PBCCategorySP& category);
};

#endif  // PBCPLAY_H
