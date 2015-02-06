#ifndef PBCPLAYER_H
#define PBCPLAYER_H

#include "util/pbcDeclarations.h"
#include "models/pbcRoute.h"
#include "models/pbcPath.h"
class PBCPlayer
{
private:
    PBCRole role;
    PBCColor color;
    PBCDPoint pos;
    PBCRouteSP route;
    std::vector<PBCPathSP> motion;



public:
    PBCPlayer();
};

typedef boost::shared_ptr<PBCPlayer> PBCPlayerSP;

#endif // PBCPLAYER_H
