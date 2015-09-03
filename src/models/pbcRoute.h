/** @file pbcRoute.h
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

#ifndef PBCROUTE_H
#define PBCROUTE_H

#include "models/pbcAbstractMovement.h"
#include "util/pbcDeclarations.h"
#include "models/pbcPath.h"

#include <boost/serialization/string.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <string>
#include <vector>

class PBCRoute;
typedef boost::shared_ptr<PBCRoute> PBCRouteSP;

class PBCRoute : public PBCAbstractMovement{
friend class boost::serialization::access;
 private:
    std::string _name;
    std::string _codeName;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {  // NOLINT
        assert(version == 0);
        ar & boost::serialization::base_object<PBCAbstractMovement>(*this);
        ar & _name;
        ar & _codeName;
    }
    PBCRoute() {}

 public:
    PBCRoute(const std::string& name,
             const std::string& codeName,
             const std::vector<PBCPathSP>& paths);
    std::string name() const;
    std::string codeName() const;
};

#endif  // PBCROUTE_H
