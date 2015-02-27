#include "pbcRoute.h"

std::list<PBCRouteSP> PBCRoute::_customRoutes = std::list<PBCRouteSP>();

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

std::vector<PBCRouteSP> PBCRoute::appendedRoutes() const
{
    return _appendedRoutes;
}

void PBCRoute::setAppendedRoutes(const std::vector<PBCRouteSP> &appendedRoutes)
{
    _appendedRoutes = appendedRoutes;
}

void PBCRoute::addCustomRoute(PBCRouteSP route)
{
    _customRoutes.push_back(route);
}

std::list<PBCRouteSP> PBCRoute::getCustomRoutes()
{
    return _customRoutes;
}


PBCRoute::PBCRoute(const std::string &name, const std::string &codeName, const std::vector<PBCPathSP> paths, std::vector<PBCRouteSP> appendedRoutes) :
    _name(name),
    _codeName(codeName),
    _paths(paths),
    _appendedRoutes(appendedRoutes) {}
