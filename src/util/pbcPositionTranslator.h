#ifndef PBCPOSITIONTRANSLATOR_H
#define PBCPOSITIONTRANSLATOR_H

#include "pbcSingleton.h"
#include "pbcDeclarations.h"
#include "pbcConfig.h"

class PBCPositionTranslator : public PBCSingleton<PBCPositionTranslator> {

friend class PBCSingleton<PBCPositionTranslator>;
private:
    static PBCDPoint ballPos();

protected:
    PBCPositionTranslator();

public:
    static PBCDPoint translatePos(PBCDPoint pos, PBCDPoint center = ballPos());
};

#endif // PBCPOSITIONTRANSLATOR_H
