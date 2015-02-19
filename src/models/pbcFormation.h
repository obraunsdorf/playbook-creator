#ifndef PBCFORMATION_H
#define PBCFORMATION_H

#include <vector>
#include <boost/shared_ptr.hpp>

#include "models/pbcCategory.h"
#include "models/pbcPlayer.h"

class PBCFormation;
typedef boost::shared_ptr<PBCFormation> PBCFormationSP;

class PBCFormation : public PBCCategory, public std::vector<PBCPlayerSP> {
public:
    PBCFormation() : PBCCategory("Standard"){
        this->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Center", "C"}, PBCColor(255,255,0), PBCDPoint(0, 0))));
        this->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Quarterback", "QB"}, PBCColor(255,0,0), PBCDPoint(0, -1))));
        this->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Wide Receiver Left", "WRL"}, PBCColor(0,255,0), PBCDPoint(-15, 0))));
        this->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Wide Receiver Right", "WRR"}, PBCColor(0,0,255), PBCDPoint(15, 0))));
        this->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Runningback", "RB"}, PBCColor(0,0,0), PBCDPoint(0, -5))));
    }

    explicit PBCFormation(const std::string& name) : PBCCategory(name) {}
};

#endif // PBCFORMATION_H
