/** @file pbcVirtualMovement.h
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

#ifndef PBCVIRTUALMOVEMENT_H
#define PBCVIRTUALMOVEMENT_H

#include "models/pbcPath.h"
#include <vector>

class PBCVirtualMovement {
friend class boost::serialization::access;
 private:
    std::vector<PBCPathSP> _paths;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {  // NOLINT
        assert(version == 0);
        ar & _paths;
    }

 protected:
    PBCVirtualMovement() {}
    explicit PBCVirtualMovement(const std::vector<PBCPathSP> paths) :
        _paths(paths) {}

 public:
    const std::vector<PBCPathSP>& paths() const {
        return _paths;
    }
    void setPaths(const std::vector<PBCPathSP> paths) {
        _paths = paths;
    }
    void addPath(const PBCPathSP& path) {
        _paths.push_back(path);
    }
};

#endif  // PBCVIRTUALMOVEMENT_H
