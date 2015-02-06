#ifndef PBCFORMATION_H
#define PBCFORMATION_H

#include <vector>
#include <boost/shared_ptr.hpp>

#include "models/pbcCategory.h"
#include "models/pbcPlayer.h"

class PBCFormation;
typedef boost::shared_ptr<PBCFormation> PBCFormationSP;

class PBCFormation : public PBCCategory {
private:
    std::vector<PBCPlayerSP> players;
};

#endif // PBCFORMATION_H
