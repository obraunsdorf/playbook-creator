#ifndef PBCFORMATION_H
#define PBCFORMATION_H

#include <vector>
#include <boost/shared_ptr.hpp>

#include "models/pbcCategory.h"
#include "models/pbcPlayer.h"

#include <boost/serialization/vector.hpp>

class PBCFormation;
typedef boost::shared_ptr<PBCFormation> PBCFormationSP;

class PBCFormation : public PBCCategory, public std::vector<PBCPlayerSP> {
friend class boost::serialization::access;

private:
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        assert(version == 0);
        ar & boost::serialization::base_object<PBCCategory>(*this);
        ar & boost::serialization::base_object<std::vector<PBCPlayerSP>>(*this);
    }
    PBCFormation() : PBCCategory("") {}

public:
    explicit PBCFormation(const std::string& name) : PBCCategory(name) {}
    explicit PBCFormation(const PBCFormation& other) : PBCCategory(other.name()), std::vector<PBCPlayerSP>() {
        for(PBCPlayerSP otherPlayer : other) {
            PBCPlayerSP player(new PBCPlayer(otherPlayer->role(), otherPlayer->color(), otherPlayer->pos()));
            this->push_back(player);
        }
    }
};

#endif // PBCFORMATION_H
