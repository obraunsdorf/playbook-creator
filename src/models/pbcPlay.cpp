#include "pbcPlay.h"


std::string PBCPlay::name() const
{
    return _name;
}

void PBCPlay::setName(const std::string &name)
{
    _name = name;
}

std::string PBCPlay::codeName() const
{
    return _codeName;
}

void PBCPlay::setCodeName(const std::string &codeName)
{
    _codeName = codeName;
}

PBCFormationSP PBCPlay::formation() const
{
    return _formation;
}

void PBCPlay::setFormation(const PBCFormationSP &formation)
{
    _formation = formation;
}

std::list<PBCCategorySP> PBCPlay::categories() const
{
    return _categories;
}

void PBCPlay::setCategories(const std::list<PBCCategorySP> &categories)
{
    _categories = categories;
}
PBCPlay::PBCPlay(const std::string &name, const std::string &codeName, const PBCFormationSP &formation) :
    _name(name),
    _codeName(codeName),
    _formation(formation) {}
