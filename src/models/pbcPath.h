#ifndef PBCPATH_H
#define PBCPATH_H

#include "util/pbcDeclarations.h"

class PBCPath
{
private:
    PBCDPoint endpoint;
    bool arc;

public:
    PBCPath();
};

typedef boost::shared_ptr<PBCPath> PBCPathSP;

#endif // PBCPATH_H
