#ifndef PBCPLAYBOOK_H
#define PBCPLAYBOOK_H

#include "util/pbcSingleton.h"
#include "models/pbcDataModel.h"
#include <list>
#include "models/pbcFormation.h"
#include "models/pbcRoute.h"
#include "models/pbcCategory.h"
#include "models/pbcPlay.h"
#include <ostream>
#include <boost/serialization/map.hpp>
#include <boost/serialization/access.hpp>

class PBCPlaybook : public PBCSingleton<PBCPlaybook>, PBCDataModel
{
friend class PBCSingleton<PBCPlaybook>;
friend class boost::serialization::access;
private:
    template<typename T> using PBCModelMap = std::map<std::string, T>;
    std::string _name;
    PBCModelMap<PBCFormationSP> _formations;
    PBCModelMap<PBCRouteSP> _routes;
    PBCModelMap<PBCCategorySP> _categories;
    PBCModelMap<PBCPlaySP> _plays;

    template<class T> std::list<T> mapToList(PBCModelMap<T> map) const {
        std::list<T> list;
        for(const auto& kv : map) {
            list.push_back(kv.second);
        }
        return list;
    }

    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        assert(version == 0);
        ar & _name;
        ar & _formations;
        ar & _routes;
        ar & _plays;
        ar & _categories;
    }
protected:
    PBCPlaybook();

public:
    void setName(const std::string& name);
    void addFormation(PBCFormationSP formation);
    void addRoute(PBCRouteSP route);
    void addCategory(PBCCategorySP category);
    void addPlay(PBCPlaySP play);
    std::string name() const;
    std::list<PBCFormationSP> formations() const;
    std::list<PBCRouteSP> routes() const;
    std::list<PBCPlaySP> plays() const;
    PBCFormationSP getFormation(const std::string& name);
    PBCPlaySP getPlay(const std::string& name);
};

#endif // PBCPLAYBOOK_H
