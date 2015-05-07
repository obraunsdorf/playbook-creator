#include "pbcMotion.h"
#include "util/pbcExceptions.h"

PBCMotion::PBCMotion() : _motionEndPoint(0, 0) {
}

void PBCMotion::addPath(const PBCPathSP& pathSP) {
    if(pathSP->endpoint().get<1>() > _motionEndPoint.get<1>()) {
        throw PBCRuleBreakException("A motion cannot go towards the LOS.");
    }
    PBCVirtualMovement::addPath(pathSP);
    _motionEndPoint = pathSP->endpoint();
}

PBCDPoint PBCMotion::motionEndPoint() const {
    return _motionEndPoint;
}
