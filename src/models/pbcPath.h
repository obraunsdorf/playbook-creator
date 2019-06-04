/** @file pbcPath.h
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

#ifndef PBCPATH_H
#define PBCPATH_H

#include "util/pbcDeclarations.h"
#include "pbcVersion.h"
#include "models/pbcPlaybook.h"
#include <boost/serialization/access.hpp>
#include <set>

class PBCPath {
friend class boost::serialization::access;
 private:
    PBCDPoint _endpoint;
    PBCDPoint _bezierControlPoint;
    bool _arc;
    bool _concave;

    template<class Archive>
    void save(Archive& ar, const unsigned int version) const {  // NOLINT
        ar << _endpoint.get<0>();
        ar << _endpoint.get<1>();
        ar << _bezierControlPoint.get<0>();
        ar << _bezierControlPoint.get<1>();
        ar << _arc;
        ar << _concave;
    }

    template<class Archive>
    void load(Archive& ar, const unsigned int version) {  //NOLINT
        double x;
        double y;
        double cx = DUMMY_POINT.get<0>();
        double cy = DUMMY_POINT.get<1>();
        ar >> x;
        ar >> y;
        if (version >= 1) {
            ar >> cx;
            ar >> cy;
        }
        ar >> _arc;
        ar >> _concave;
        _endpoint.set<0>(x);
        _endpoint.set<1>(y);
        _bezierControlPoint.set<0>(cx);
        _bezierControlPoint.set<1>(cy);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    PBCPath() {}

 public:
    explicit PBCPath(PBCDPoint endpoint,
                     bool arc = false,
                     bool concave = false,
                     PBCDPoint controlPoint = DUMMY_POINT);
    PBCPath(double endpointX,
            double endpointY,
            bool arc = false,
            bool concave = false,
            double controlX = DUMMY_POINT.get<0>(),
            double controlY = DUMMY_POINT.get<1>());
    PBCDPoint endpoint() const;
    PBCDPoint bezierControlPoint() const;
    bool isArc() const;
    bool isConcave() const;
};
BOOST_CLASS_VERSION(PBCPath, 1)

typedef boost::shared_ptr<PBCPath> PBCPathSP;

#endif  // PBCPATH_H
