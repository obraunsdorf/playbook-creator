#include "pbcRoute.h"

std::string PBCRoute::name() const
{
    return _name;
}

std::string PBCRoute::codeName() const
{
    return _codeName;
}

std::vector<PBCPathSP> PBCRoute::paths() const
{
    return _paths;
}

void PBCRoute::setPaths(const std::vector<PBCPathSP> &paths)
{
    _paths = paths;
}

PBCRoute::PBCRoute(const std::string &name, const std::string &codeName, const std::vector<PBCPathSP>& paths) :
    _name(name),
    _codeName(codeName),
    _paths(paths) {}
