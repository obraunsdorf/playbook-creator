/** @file pbcMotion.h
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

#ifndef PBCMOTION_H
#define PBCMOTION_H

#include "models/pbcAbstractMovement.h"
#include "models/pbcPath.h"
#include <boost/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>
#include <set>

class PBCMotion;
typedef boost::shared_ptr<PBCMotion> PBCMotionSP;

class PBCMotion : public PBCAbstractMovement {
friend class boost::serialization::access;
 private:
    PBCDPoint _motionEndPoint;

    template<class Archive>
    void save(Archive& ar, const unsigned int version) const {  // NOLINT
        pbcAssert(version == 0);
        ar << boost::serialization::base_object<PBCAbstractMovement>(*this);
        ar << _motionEndPoint.get<0>();
        ar << _motionEndPoint.get<1>();
    }

    template<class Archive>
    void load(Archive& ar, const unsigned int version) {  // NOLINT
        pbcAssert(version == 0);
        ar >> boost::serialization::base_object<PBCAbstractMovement>(*this);
        double x;
        double y;
        ar >> x;
        ar >> y;
        _motionEndPoint.set<0>(x);
        _motionEndPoint.set<1>(y);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

 public:
    PBCMotion(const std::vector<PBCPathSP>& paths =  std::vector<PBCPathSP>());
    void addPath(const PBCPathSP& pathSP);
    PBCDPoint motionEndPoint() const;
};

#endif  // PBCMOTION_H
