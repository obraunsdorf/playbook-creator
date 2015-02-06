#ifndef PBCPLAY_H
#define PBCPLAY_H

#include <string>
#include <vector>

#include "util/pbcDeclarations.h"
#include "models/pbcPlayer.h"
#include "models/pbcCategory.h"
#include "models/pbcFormation.h"

class PBCPlay;
typedef boost::shared_ptr<PBCPlay> PBCPlaySP;

class PBCPlay
{

private:
    std::string name;
    std::string codeName;
    PBCFormationSP formation;
    std::vector<PBCPlayerSP> players;
    std::list<PBCCategorySP> categories;


public:
    PBCPlay(const std::string& name, const std::string& codeName);
    std::string getName() const;
    void setName(const std::string &value);
    std::string getCodeName() const;
    void setCodeName(const std::string &value);
};

typedef boost::shared_ptr<PBCPlay> PBCPlaySP;



#endif // PBCPLAY_H
