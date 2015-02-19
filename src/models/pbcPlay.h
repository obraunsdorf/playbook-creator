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
    std::string _name;
    std::string _codeName;
    PBCFormationSP _formation;
    std::list<PBCCategorySP> _categories;


public:
    PBCPlay(const std::string& name = "Standard", const std::string& codeName = "", const PBCFormationSP& formation = PBCFormationSP(new PBCFormation()));
    std::string name() const;
    void setName(const std::string &name);
    std::string codeName() const;
    void setCodeName(const std::string &codeName);
    PBCFormationSP formation() const;
    void setFormation(const PBCFormationSP &formation);
    std::list<PBCCategorySP> categories() const;
    void setCategories(const std::list<PBCCategorySP> &categories);
};

typedef boost::shared_ptr<PBCPlay> PBCPlaySP;



#endif // PBCPLAY_H
