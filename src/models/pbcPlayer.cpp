#include "pbcPlayer.h"


PBCRole PBCPlayer::role() const {
    return _role;
}

void PBCPlayer::setRole(const PBCRole &role) {
    _role = role;
}

PBCColor PBCPlayer::color() const {
    return _color;
}

void PBCPlayer::setColor(const PBCColor &color) {
    _color = color;
}

PBCDPoint PBCPlayer::pos() const {
    return _pos;
}

void PBCPlayer::setPos(const PBCDPoint &pos) {
    _pos = pos;
}

PBCRouteSP PBCPlayer::route() const {
    return _route;
}

void PBCPlayer::setRoute(const PBCRouteSP &route) {
    _route = route;
}

PBCMotionSP PBCPlayer::motion() const {
    return _motion;
}

void PBCPlayer::setMotion(const PBCMotionSP &motion) {
    _motion = motion;
}

PBCPlayer::PBCPlayer(PBCRole role,
                     PBCColor color,
                     PBCDPoint pos,
                     PBCRouteSP route,
                     PBCMotionSP motion) :
    _role(role),
    _color(color),
    _pos(pos),
    _route(route),
    _motion(motion) {}


