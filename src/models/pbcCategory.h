#ifndef PBCCATEGORY_H
#define PBCCATEGORY_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <assert.h>

class PBCPlay;
typedef boost::shared_ptr<PBCPlay> PBCPlaySP;

class PBCCategory;
typedef boost::shared_ptr<PBCCategory> PBCCategorySP;

class PBCCategory {
protected:
    std::string _name;
    // char _shortName[5];
    std::list<PBCPlaySP> _plays;

public:
    PBCCategory(const std::string& name) : _name(name) {}
};

#endif // PBCCATEGORY_H
