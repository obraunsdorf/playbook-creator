#ifndef PBCPLAYER_H
#define PBCPLAYER_H

#include "util/pbcDeclarations.h"
#include "models/pbcRoute.h"
#include "models/pbcPath.h"
#include "models/pbcColor.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/shared_ptr.hpp>

class PBCPlayer;
typedef std::vector<PBCPathSP> PBCMotion;

class PBCPlayer  : public PBCDataModel
{
friend class boost::serialization::access;
private:
    template<class Archive> void save(Archive& ar, const unsigned int version) const {
        assert(version == 0);
        ar << _role.fullName;
        ar << _role.shortName;
        ar << _color;
        ar << _pos.get<0>();
        ar << _pos.get<1>();
        ar << _route;
        // todo motion
    }
    template<class Archive> void load(Archive& ar, const unsigned int version) {
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
        _role.fullName = fullName;
        _role.shortName = shortName;
        _pos.set<0>(x);
        _pos.set<1>(y);
        //todo motion
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    PBCPlayer() {}

    PBCRole _role;
    PBCColor _color;
    PBCDPoint _pos;
    PBCRouteSP _route;
    PBCMotion _motion;

public:
    PBCPlayer(PBCRole role, PBCColor color, PBCDPoint pos = PBCDPoint(), PBCRouteSP route = NULL, PBCMotion motion = PBCMotion());
    PBCRole role() const;
    void setRole(const PBCRole &role);
    PBCColor color() const;
    void setColor(const PBCColor &color);
    PBCDPoint pos() const;
    void setPos(const PBCDPoint &pos);
    PBCRouteSP route() const;
    void setRoute(const PBCRouteSP &route);
    PBCMotion motion() const;
    void setMotion(const PBCMotion &motion);

    void serialize(std::ostream &ostream);
};

typedef boost::shared_ptr<PBCPlayer> PBCPlayerSP;

#endif // PBCPLAYER_H
