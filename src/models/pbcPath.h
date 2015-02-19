#ifndef PBCPATH_H
#define PBCPATH_H

#include "util/pbcDeclarations.h"

class PBCPath
{
private:
    PBCDPoint _endpoint;
    bool _arc;

public:
    PBCPath(PBCDPoint endpoint, bool arc = false);
    PBCPath(double endpointX, double endpointY, bool arc = false);
    PBCDPoint endpoint() const;
};

typedef boost::shared_ptr<PBCPath> PBCPathSP;

#endif // PBCPATH_H
