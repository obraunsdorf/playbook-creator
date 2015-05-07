#ifndef PBCMOTION_H
#define PBCMOTION_H

#include "models/pbcVirtualMovement.h"
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

class PBCMotion : public PBCVirtualMovement {
friend class boost::serialization::access;
 private:
    PBCDPoint _motionEndPoint;

    template<class Archive>
    void save(Archive& ar, const unsigned int version) const {  // NOLINT
        assert(version == 0);
        ar << boost::serialization::base_object<PBCVirtualMovement>(*this);
        ar << _motionEndPoint.get<0>();
        ar << _motionEndPoint.get<1>();
    }

    template<class Archive>
    void load(Archive& ar, const unsigned int version) {  // NOLINT
        assert(version == 0);
        ar >> boost::serialization::base_object<PBCVirtualMovement>(*this);
        double x;
        double y;
        ar >> x;
        ar >> y;
        _motionEndPoint.set<0>(x);
        _motionEndPoint.set<1>(y);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

 public:
    PBCMotion();
    void addPath(const PBCPathSP& pathSP);
    PBCDPoint motionEndPoint() const;
};

#endif  // PBCMOTION_H
