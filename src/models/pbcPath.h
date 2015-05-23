#ifndef PBCPATH_H
#define PBCPATH_H

#include "util/pbcDeclarations.h"
#include "pbcVersion.h"
#include "models/pbcPlaybook.h"
#include <boost/serialization/access.hpp>
#include <set>

class PBCPath {
friend class boost::serialization::access;
 private:
    PBCDPoint _endpoint;
    bool _arc;
    bool _concave;

    template<class Archive>
    void save(Archive& ar, const unsigned int version) const {  // NOLINT
        assert(version == 0);
        ar << _endpoint.get<0>();
        ar << _endpoint.get<1>();
        ar << _arc;
        ar << _concave;
    }

    template<class Archive>
    void load(Archive& ar, const unsigned int version) {  //NOLINT
        assert(version == 0);
        double x;
        double y;
        ar >> x;
        ar >> y;
        ar >> _arc;
        ar >> _concave;
        _endpoint.set<0>(x);
        _endpoint.set<1>(y);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    PBCPath() {}

 public:
    explicit PBCPath(PBCDPoint endpoint,
                     bool arc = false,
                     bool concave = false);
    PBCPath(double endpointX,
            double endpointY,
            bool arc = false,
            bool concave = false);
    PBCDPoint endpoint() const;
    bool isArc() const;
    bool isConcave() const;
};

typedef boost::shared_ptr<PBCPath> PBCPathSP;

#endif  // PBCPATH_H
