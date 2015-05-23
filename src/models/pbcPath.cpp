#include "pbcPath.h"


PBCDPoint PBCPath::endpoint() const {
    return _endpoint;
}

bool PBCPath::isArc() const {
    return _arc;
}

bool PBCPath::isConcave() const {
    return _concave;
}

PBCPath::PBCPath(PBCDPoint endpoint, bool arc, bool concave) :
    _endpoint(endpoint),
    _arc(arc),
    _concave(concave) {}

PBCPath::PBCPath(double endpointX, double endpointY, bool arc, bool concave) :
    _endpoint(PBCDPoint(endpointX, endpointY)),
    _arc(arc),
    _concave(concave) {}
