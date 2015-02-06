#include "pbcPlay.h"


std::string PBCPlay::getName() const
{
    return name;
}

void PBCPlay::setName(const std::string &value)
{
    name = value;
}

std::string PBCPlay::getCodeName() const
{
    return codeName;
}

void PBCPlay::setCodeName(const std::string &value)
{
    codeName = value;
}
PBCPlay::PBCPlay(const std::string &name, const std::string &codeName)
{
    this->name = name;
    this->codeName = codeName;
}
