#include "pbcPlaybook.h"

PBCPlaybook::PBCPlaybook() : _name("Playbook")
{
    std::vector<PBCPathSP> path_5In({ PBCPathSP(new PBCPath(0,5)), PBCPathSP(new PBCPath(2,5))});
    PBCRouteSP route_5In(new PBCRoute("5 In", "", path_5In));
    _routes.insert(std::make_pair(route_5In->name(), route_5In));

    std::vector<PBCPathSP> path_Post({ PBCPathSP(new PBCPath(0,5)), PBCPathSP(new PBCPath(2,7))});
    PBCRouteSP route_Post(new PBCRoute("Post", "", path_Post));
    _routes.insert(std::make_pair(route_Post->name(), route_Post));

    std::vector<PBCPathSP> path_Slant({ PBCPathSP(new PBCPath(0,2)), PBCPathSP(new PBCPath(2,4))});
    PBCRouteSP route_Slant(new PBCRoute("Slant", "", path_Slant));
    _routes.insert(std::make_pair(route_Slant->name(), route_Slant));

    std::vector<PBCPathSP> path_Fly({ PBCPathSP(new PBCPath(0,7))});
    PBCRouteSP route_Fly(new PBCRoute("Fly", "", path_Fly));
    _routes.insert(std::make_pair(route_Fly->name(), route_Fly));

    PBCFormationSP formation(new PBCFormation("PBC_StandardFormation"));
    formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Center", "C"}, PBCColor(255,255,0), PBCDPoint(0, 0))));
    formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Quarterback", "QB"}, PBCColor(255,0,0), PBCDPoint(0, -1))));
    formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Wide Receiver Left", "WRL"}, PBCColor(0,255,0), PBCDPoint(-15, 0))));
    formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Wide Receiver Right", "WRR"}, PBCColor(0,0,255), PBCDPoint(15, 0))));
    formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Runningback", "RB"}, PBCColor(0,0,0), PBCDPoint(0, -5))));
    std::pair<PBCModelMap<PBCFormationSP>::iterator, bool> result = _formations.insert(std::make_pair(formation->name(), formation));
    assert(result.second == true);
}

void PBCPlaybook::setName(const std::string &name)
{
    _name = name;
}

void PBCPlaybook::addFormation(PBCFormationSP formation)
{
    std::pair<PBCModelMap<PBCFormationSP>::iterator, bool> result = _formations.insert(std::make_pair(formation->name(), PBCFormationSP(new PBCFormation(*formation))));
    if(result.second == false) {
        // TODO message to user: same name as existing <model>
    }
}

void PBCPlaybook::addRoute(PBCRouteSP route)
{
    std::pair<PBCModelMap<PBCRouteSP>::iterator, bool> result = _routes.insert(std::make_pair(route->name(), route));
    if(result.second == false) {
        // TODO message to user: same name as existing <model>
    }
}

void PBCPlaybook::addCategory(PBCCategorySP category)
{
    std::pair<PBCModelMap<PBCCategorySP>::iterator, bool> result = _categories.insert(std::make_pair(category->name(), category));
    if(result.second == false) {
        // TODO message to user: same name as existing <model>
    }
}

void PBCPlaybook::addPlay(PBCPlaySP play)
{
    std::pair<PBCModelMap<PBCPlaySP>::iterator, bool> result = _plays.insert(std::make_pair(play->name(), PBCPlaySP(new PBCPlay(*play))));
    if(result.second == false) {
        // TODO message to user: same name as existing <model>
    }
}

std::string PBCPlaybook::name() const
{
    return _name;
}

std::list<PBCFormationSP> PBCPlaybook::formations() const
{
    return mapToList<PBCFormationSP>(_formations);
}

std::list<PBCRouteSP> PBCPlaybook::routes() const
{
    return mapToList<PBCRouteSP>(_routes);
}

std::list<PBCPlaySP> PBCPlaybook::plays() const
{
    return mapToList<PBCPlaySP>(_plays);
}

PBCFormationSP PBCPlaybook::getFormation(const std::string &name)
{
    const auto& it = _formations.find(name);
    assert(it != _formations.end()); // todo throw exception
    PBCFormationSP formation = it->second;
    return PBCFormationSP(new PBCFormation(*formation));
}

PBCPlaySP PBCPlaybook::getPlay(const std::string &name)
{
    const auto& it = _plays.find(name);
    assert(it != _plays.end()); // todo throw exception
    PBCPlaySP play = it->second;
    return PBCPlaySP(new PBCPlay(*play));
}
