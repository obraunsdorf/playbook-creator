#ifndef PBCROUTE_H
#define PBCROUTE_H

#include "util/pbcDeclarations.h"
#include "models/pbcPath.h"

class PBCRoute;
typedef boost::shared_ptr<PBCRoute> PBCRouteSP;

class PBCRoute
{
private:
    std::string name;
    std::string codeName;
    std::vector<PBCPathSP> paths;
    std::vector<PBCRouteSP> appendedRoutes;

public:
    PBCRoute();
};

#endif // PBCROUTE_H
