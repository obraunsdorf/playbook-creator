#include "pbcPlay.h"
#include <string>
#include <list>


std::string PBCPlay::name() const {
    return _name;
}

void PBCPlay::setName(const std::string &name) {
    _name = name;
}

std::string PBCPlay::codeName() const {
    return _codeName;
}

void PBCPlay::setCodeName(const std::string &codeName) {
    _codeName = codeName;
}

PBCFormationSP PBCPlay::formation() const {
    return _formation;
}

void PBCPlay::setFormation(const PBCFormationSP &formation) {
    _formation = formation;
}

std::list<PBCCategorySP> PBCPlay::categories() const {
    return _categories;
}

void PBCPlay::setCategories(const std::list<PBCCategorySP> &categories) {
    _categories = categories;
}

PBCPlay::PBCPlay(const std::string &name,
                 const std::string &codeName,
                 const std::string& formationName) :
    _name(name),
    _codeName(codeName),
    _formation(PBCPlaybook::getInstance()->getFormation(formationName)) {}

PBCPlay::PBCPlay(const PBCPlay &other) :
    _name(other.name()),
    _codeName(other.codeName()),
    _formation(new PBCFormation(*other.formation())),
    _categories(other.categories()) {
    PBCFormation::iterator it = _formation->begin();
    PBCFormation::iterator otherIt = other.formation()->begin();
    while(it != _formation->end() && otherIt != other.formation()->end()) {
        PBCPlayerSP playerSP = *it;
        PBCPlayerSP otherPlayerSP = *otherIt;
        playerSP->setRoute(otherPlayerSP->route());
        ++it;
        ++otherIt;
    }
    assert(it == _formation->end() && otherIt == other.formation()->end());
}
