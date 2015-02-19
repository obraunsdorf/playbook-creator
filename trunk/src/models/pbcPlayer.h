#ifndef PBCPLAYER_H
#define PBCPLAYER_H

#include "util/pbcDeclarations.h"
#include "models/pbcRoute.h"
#include "models/pbcPath.h"
class PBCPlayer;
typedef std::vector<PBCPathSP> PBCMotion;

class PBCPlayer
{
private:
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
};

typedef boost::shared_ptr<PBCPlayer> PBCPlayerSP;

#endif // PBCPLAYER_H
