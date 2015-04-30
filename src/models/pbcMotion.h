#ifndef PBCMOTION_H
#define PBCMOTION_H

#include "models/pbcPath.h"
#include <boost/shared_ptr.hpp>
#include <vector>

class PBCMotion;
typedef boost::shared_ptr<PBCMotion> PBCMotionSP;
class PBCMotion : private std::vector<PBCPathSP> {
 private:
    typedef std::vector<PBCPathSP>::iterator iterator;
    typedef std::vector<PBCPathSP>::const_iterator const_iterator;
    PBCDPoint _motionEndPoint;
 public:
    PBCMotion();
    void addPath(PBCPathSP pathSP);
    PBCDPoint motionEndPoint() const;
    iterator begin() { return std::vector<PBCPathSP>::begin(); }
    const_iterator begin() const { return std::vector<PBCPathSP>::begin(); }
    iterator end() { return std::vector<PBCPathSP>::end(); }
    const_iterator end() const { return std::vector<PBCPathSP>::end(); }
};

#endif  // PBCMOTION_H
