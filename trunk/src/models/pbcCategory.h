#ifndef PBCCATEGORY_H
#define PBCCATEGORY_H

#include <string>
#include <boost/shared_ptr.hpp>

class PBCPlay;
typedef boost::shared_ptr<PBCPlay> PBCPlaySP;

class PBCCategory;
typedef boost::shared_ptr<PBCCategory> PBCCategorySP;

class PBCCategory {
private:
    std::string name;
    char shortName[5];
    std::list<PBCPlaySP> plays;
};

#endif // PBCCATEGORY_H
