#ifndef PBCROUTE_H
#define PBCROUTE_H

#include "util/pbcDeclarations.h"
#include "models/pbcPath.h"

class PBCRoute;
typedef boost::shared_ptr<PBCRoute> PBCRouteSP;

class PBCRoute
{
private:
    std::string _name;
    std::string _codeName;
    std::vector<PBCPathSP> _paths;
    std::vector<PBCRouteSP> _appendedRoutes;

    static std::list<PBCRouteSP> _customRoutes;

public:
    PBCRoute(const std::string& name, const std::string& codeName, const std::vector<PBCPathSP> paths = std::vector<PBCPathSP>(), std::vector<PBCRouteSP> appendedRoutes = std::vector<PBCRouteSP>());
    std::string name() const;
    std::string codeName() const;
    std::vector<PBCPathSP> paths() const;
    void setPaths(const std::vector<PBCPathSP> &paths);
    std::vector<PBCRouteSP> appendedRoutes() const;
    void setAppendedRoutes(const std::vector<PBCRouteSP> &appendedRoutes);

    static void addCustomRoute(PBCRouteSP route);
    static std::list<PBCRouteSP> getCustomRoutes();

};

#endif // PBCROUTE_H
