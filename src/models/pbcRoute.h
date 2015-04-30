#ifndef PBCROUTE_H
#define PBCROUTE_H

#include "util/pbcDeclarations.h"
#include "models/pbcPath.h"

#include <boost/serialization/string.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <string>
#include <vector>

class PBCRoute;
typedef boost::shared_ptr<PBCRoute> PBCRouteSP;

class PBCRoute {
friend class boost::serialization::access;
 private:
    std::string _name;
    std::string _codeName;
    std::vector<PBCPathSP> _paths;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {  // NOLINT
        assert(version == 0);
        ar & _name;
        ar & _codeName;
        ar & _paths;
    }
    PBCRoute() {}

 public:
    PBCRoute(const std::string& name,
             const std::string& codeName,
             const std::vector<PBCPathSP>& paths = std::vector<PBCPathSP>());
    std::string name() const;
    std::string codeName() const;
    std::vector<PBCPathSP> paths() const;
    void setPaths(const std::vector<PBCPathSP> &paths);
};

#endif  // PBCROUTE_H
