#ifndef PBCPLAYBOOK_H
#define PBCPLAYBOOK_H

#include "util/pbcSingleton.h"
#include <list>
#include "models/pbcFormation.h"
#include "models/pbcRoute.h"
#include "models/pbcCategory.h"
#include "models/pbcPlay.h"
#include <ostream>
#include <boost/serialization/map.hpp>
#include <boost/serialization/access.hpp>
#include <map>
#include <string>
#include <utility>
#include <vector>

class PBCPlaybook : public PBCSingleton<PBCPlaybook> {
friend class PBCSingleton<PBCPlaybook>;
friend class boost::serialization::access;
 private:
    template<typename T> using PBCModelMap = std::map<std::string, T>;
    std::string _name;
    PBCModelMap<PBCFormationSP> _formations;
    PBCModelMap<PBCRouteSP> _routes;
    PBCModelMap<PBCCategorySP> _categories;
    PBCModelMap<PBCPlaySP> _plays;


    template<typename T>
    using InsertResult = std::pair<typename PBCModelMap<T>::iterator, bool>;

    template<class T>
    std::list<T> mapToList(PBCModelMap<T> map) const {
        std::list<T> list;
        for(const auto& kv : map) {
            list.push_back(kv.second);
        }
        return list;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {  // NOLINT
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
    void resetToNewEmptyPlaybook(const std::string& name);
    void setName(const std::string& name);
    bool addFormation(PBCFormationSP formation, bool overwrite = false);
    bool addRoute(PBCRouteSP route, bool overwrite = false);
    bool addCategory(PBCCategorySP category, bool overwrite = false);
    bool addPlay(PBCPlaySP play, bool overwrite = false);
    std::string name() const;
    std::list<PBCFormationSP> formations() const;
    std::list<PBCRouteSP> routes() const;
    std::list<PBCPlaySP> plays() const;
    PBCFormationSP getFormation(const std::string& name);
    PBCPlaySP getPlay(const std::string& name);
    std::vector<std::string> getFormationNames() const;
    std::vector<std::string> getPlayNames() const;
};

#endif  // PBCPLAYBOOK_H
