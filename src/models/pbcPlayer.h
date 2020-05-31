/** @file pbcPlayer.h
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

#ifndef PBCPLAYER_H
#define PBCPLAYER_H

#include "util/pbcDeclarations.h"
#include "models/pbcRoute.h"
#include "models/pbcMotion.h"
#include "models/pbcPath.h"
#include "models/pbcColor.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <string>
#include <vector>
#include <array>
#include <set>

class PBCPlayer;
typedef boost::shared_ptr<PBCPlayer> PBCPlayerSP;

/**
 * @brief A role is a synonym for the players position.
 *
 * fullName is the long term, e.g. "Quarterback".
 * shortname is the short term, e.g. "QB". It is limited to four chars.
 */
struct PBCRole {
    std::string fullName;
    std::array<char, 4> shortName;
};

class PBCPlayer {
friend class boost::serialization::access;
 private:
    PBCRole _role;
    PBCColor _color;
    PBCDPoint _pos;
    PBCRouteSP _route;
    PBCMotionSP _motion;
    std::string _name;
    unsigned int _nr;

    template<class Archive>
    void save(Archive& ar, const unsigned int version) const {  // NOLINT
        ar << _role.fullName;
        ar << _role.shortName;
        ar << _color;
        ar << _pos.get<0>();
        ar << _pos.get<1>();
        ar << _route;
        ar << _motion;
        ar << _name;
        ar << _nr;
    }

    template<class Archive>
    void load(Archive& ar, const unsigned int version) {  // NOLINT
        std::string fullName;
        std::array<char, 4> shortName;
        double x;
        double y;
        ar >> fullName;
        ar >> shortName;
        ar >> _color;
        ar >> x;
        ar >> y;
        ar >> _route;
        ar >> _motion;
        _role.fullName = fullName;
        _role.shortName = shortName;
        _pos.set<0>(x);
        _pos.set<1>(y);
        if (version >= 1) {
            ar >> _name;
            ar >> _nr;
        }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    PBCPlayer() :
        _pos(PBCDPoint()),
        _name(""),
        _nr(0) {}

 public:
    PBCPlayer(PBCRole role,
              PBCColor color,
              PBCDPoint pos = PBCDPoint(),
              const std::string& name = "",
              unsigned int nr = 0,  // 0 is the discriminator. Valid player Numbers start from 1.
                // Maybe circumvent this "dirty hack" by using C++17 optionals
              PBCRouteSP route = NULL,
              PBCMotionSP motion = NULL);

    const std::string &name() const;
    void setName(const std::string &name);
    unsigned int nr() const;
    void setNr(unsigned int nr);

    PBCRole role() const;
    void setRole(const PBCRole &role);
    PBCColor color() const;
    void setColor(const PBCColor &color);
    PBCDPoint pos() const;
    void setPos(const PBCDPoint &pos);
    PBCRouteSP route() const;
    void setRoute(const PBCRouteSP &route);
    PBCMotionSP motion() const;
    void setMotion(const PBCMotionSP &motion);
};
BOOST_CLASS_VERSION(PBCPlayer, 1)

#endif  // PBCPLAYER_H
