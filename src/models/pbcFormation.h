/** @file pbcFormation.h
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

#ifndef PBCFORMATION_H
#define PBCFORMATION_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <string>
#include "models/pbcCategory.h"
#include "models/pbcPlayer.h"
#include <boost/serialization/vector.hpp>


class PBCFormation;
typedef boost::shared_ptr<PBCFormation> PBCFormationSP;

/**
 * @class PBCFormation
 * @brief A model class that stores information about a formation
 *
 * A formation is a vector of players (which store the position information) and
 * also a category.
 */
class PBCFormation : public PBCCategory, public std::vector<PBCPlayerSP> {
friend class boost::serialization::access;

 private:
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {  // NOLINT
        pbcAssert(version == 0);
        ar & boost::serialization::base_object<PBCCategory>(*this);
        ar & boost::serialization::base_object<std::vector<PBCPlayerSP>>(*this);
    }
    PBCFormation() : PBCCategory("") {}

 public:
    explicit PBCFormation(const std::string& name) : PBCCategory(name) {}

    /**
     * @brief The copy constructor
     *
     * Copies the formation, which means copying the players without their
     * routes and motions.
     * @param other The formation to copy
     */
    explicit PBCFormation(const PBCFormation& other) :
        PBCCategory(other.name()), std::vector<PBCPlayerSP>() {
        for(PBCPlayerSP otherPlayer : other) {
            PBCPlayerSP player(new PBCPlayer(otherPlayer->role(),
                                             otherPlayer->color(),
                                             otherPlayer->pos(),
                                             otherPlayer->name(),
                                             otherPlayer->nr()));
            this->push_back(player);
        }
    }
};

#endif  // PBCFORMATION_H
