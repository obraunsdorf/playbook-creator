#include "pbcRoute.h"
#include <string>
#include <vector>

PBCRoute::PBCRoute(const std::string &name,
                   const std::string &codeName,
                   const std::vector<PBCPathSP>& paths) :
    PBCVirtualMovement(paths),
    _name(name),
    _codeName(codeName) {}

std::string PBCRoute::name() const {
    return _name;
}

std::string PBCRoute::codeName() const {
    return _codeName;
}
