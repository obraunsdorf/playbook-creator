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
#include <set>

class PBCPlayer;
typedef boost::shared_ptr<PBCPlayer> PBCPlayerSP;

class PBCPlayer {
friend class boost::serialization::access;
 private:
    PBCRole _role;
    PBCColor _color;
    PBCDPoint _pos;
    PBCRouteSP _route;
    PBCMotionSP _motion;

    template<class Archive>
    void save(Archive& ar, const unsigned int version) const {  // NOLINT
        assert(version == 0);
        ar << _role.fullName;
        ar << _role.shortName;
        ar << _color;
        ar << _pos.get<0>();
        ar << _pos.get<1>();
        ar << _route;
        ar << _motion;
    }

    template<class Archive>
    void load(Archive& ar, const unsigned int version) {  // NOLINT
        assert(version == 0);
        std::string fullName;
        boost::array<char, 4> shortName;
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
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    PBCPlayer() {}

 public:
    PBCPlayer(PBCRole role,
              PBCColor color,
              PBCDPoint pos = PBCDPoint(),
              PBCRouteSP route = NULL,
              PBCMotionSP motion = NULL);
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

#endif  // PBCPLAYER_H
