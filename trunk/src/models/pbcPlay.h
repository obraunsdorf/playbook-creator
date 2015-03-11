#ifndef PBCPLAY_H
#define PBCPLAY_H

#include <string>
#include <vector>

#include "models/pbcPlaybook.h"
#include "util/pbcDeclarations.h"
#include "models/pbcPlayer.h"
#include "models/pbcCategory.h"
#include "models/pbcFormation.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>

class PBCPlay;
typedef boost::shared_ptr<PBCPlay> PBCPlaySP;

class PBCPlay : public PBCDataModel
{
friend class boost::serialization::access;

private:
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        assert(version == 0);
        ar & _name;
        ar & _codeName;
        ar & _formation;
        ar & _categories;
    }

    std::string _name;
    std::string _codeName;
    PBCFormationSP _formation;
    std::list<PBCCategorySP> _categories;


public:
    PBCPlay(const std::string& name = "PBC_StandardPlay", const std::string& codeName = "",
            const std::string& formationName = "PBC_StandardFormation");
    PBCPlay(const PBCPlay& other);
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
