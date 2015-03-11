#ifndef PBCCATEGORY_H
#define PBCCATEGORY_H

#include "models/pbcDataModel.h"

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>
#include <assert.h>

class PBCPlay;
typedef boost::shared_ptr<PBCPlay> PBCPlaySP;

class PBCCategory;
typedef boost::shared_ptr<PBCCategory> PBCCategorySP;

class PBCCategory : public PBCDataModel {
friend class boost::serialization::access;

private:
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        assert(version == 0);
        ar & _name;
        ar & _plays;
    }
    // PBCCategory() {}

protected:
    PBCCategory() {}
    std::string _name;
    // char _shortName[5];
    std::list<PBCPlaySP> _plays;

public:
    PBCCategory(const std::string& name) : _name(name) {}
    std::string name() const {return _name;}
    void setName(const std::string& name) {_name = name;}
};

#endif // PBCCATEGORY_H
