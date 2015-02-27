#include "pbcPositionTranslator.h"

PBCPositionTranslator::PBCPositionTranslator()
{
}

PBCDPoint PBCPositionTranslator::ballPos() {
    unsigned int y = PBCConfig::getInstance()->losY();
    unsigned int x = PBCConfig::getInstance()->canvasWidth() / 2;
    return PBCDPoint(x,y);
}


PBCDPoint PBCPositionTranslator::translatePos(PBCDPoint pos, PBCDPoint center) {
    unsigned int factor = PBCConfig::getInstance()->ydInPixel();
    assert(factor > 0);
    return PBCDPoint(center.get<0>() + factor * pos.get<0>(), center.get<1>() - factor * pos.get<1>());
}


PBCDPoint PBCPositionTranslator::retranslatePos(PBCDPoint pos, PBCDPoint center) {
    unsigned int factor = PBCConfig::getInstance()->ydInPixel();
    assert(factor > 0);
    double x = (pos.get<0>() - center.get<0>()) / (double) factor;
    double y = (pos.get<1>() - center.get<1>()) / (double) factor * (-1);
    return PBCDPoint(x, y);
}
