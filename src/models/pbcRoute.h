#ifndef PBCROUTE_H
#define PBCROUTE_H

#include "models/pbcVirtualMovement.h"
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

class PBCRoute : public PBCVirtualMovement{
friend class boost::serialization::access;
 private:
    std::string _name;
    std::string _codeName;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {  // NOLINT
        assert(version == 0);
        ar & boost::serialization::base_object<PBCVirtualMovement>(*this);
        ar & _name;
        ar & _codeName;
    }
    PBCRoute() {}

 public:
    PBCRoute(const std::string& name,
             const std::string& codeName,
             const std::vector<PBCPathSP>& paths);
    std::string name() const;
    std::string codeName() const;
};

#endif  // PBCROUTE_H
