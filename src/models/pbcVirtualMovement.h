#ifndef PBCVIRTUALMOVEMENT_H
#define PBCVIRTUALMOVEMENT_H

#include "models/pbcPath.h"
#include <vector>

class PBCVirtualMovement {
friend class boost::serialization::access;
 private:
    std::vector<PBCPathSP> _paths;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {  // NOLINT
        assert(version == 0);
        ar & _paths;
    }

 protected:
    PBCVirtualMovement() {}
    explicit PBCVirtualMovement(const std::vector<PBCPathSP> paths) :
        _paths(paths) {}

 public:
    const std::vector<PBCPathSP>& paths() const {
        return _paths;
    }
    void setPaths(const std::vector<PBCPathSP> paths) {
        _paths = paths;
    }
    void addPath(const PBCPathSP& path) {
        _paths.push_back(path);
    }
};

#endif  // PBCVIRTUALMOVEMENT_H
