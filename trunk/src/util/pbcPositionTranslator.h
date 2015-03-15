#ifndef PBCPOSITIONTRANSLATOR_H
#define PBCPOSITIONTRANSLATOR_H

#include "pbcSingleton.h"
#include "pbcDeclarations.h"
#include "pbcConfig.h"

class PBCPositionTranslator : public PBCSingleton<PBCPositionTranslator> {
friend class PBCSingleton<PBCPositionTranslator>;
 private:
    // cppcheck-suppress unusedPrivateFunction
    static PBCDPoint ballPos();

 protected:
    PBCPositionTranslator();

 public:
    static PBCDPoint translatePos(PBCDPoint pos, PBCDPoint center = ballPos());
    static PBCDPoint retranslatePos(PBCDPoint pos,
                                    PBCDPoint center = ballPos());
};

#endif  // PBCPOSITIONTRANSLATOR_H
