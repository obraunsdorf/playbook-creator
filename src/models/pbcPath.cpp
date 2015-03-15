#include "pbcPath.h"


PBCDPoint PBCPath::endpoint() const {
    return _endpoint;
}

PBCPath::PBCPath(PBCDPoint endpoint, bool arc) :
    _endpoint(endpoint), _arc(arc) {}

PBCPath::PBCPath(double endpointX, double endpointY, bool arc) :
    _endpoint(PBCDPoint(endpointX, endpointY)), _arc(arc) {}
