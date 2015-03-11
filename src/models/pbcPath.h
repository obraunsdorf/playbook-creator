#ifndef PBCPATH_H
#define PBCPATH_H

#include "models/pbcDataModel.h"
#include "util/pbcDeclarations.h"
#include <boost/serialization/access.hpp>

class PBCPath  : public PBCDataModel
{
friend class boost::serialization::access;

private:
    template<class Archive> void save(Archive& ar, const unsigned int version) const {
        assert(version == 0);
        ar << _endpoint.get<0>();
        ar << _endpoint.get<1>();
        ar << _arc;
    }
    template<class Archive> void load(Archive& ar, const unsigned int version) {
        assert(version == 0);
        double x;
        double y;
        ar >> x;
        ar >> y;
        ar >> _arc;
        _endpoint.set<0>(x);
        _endpoint.set<1>(y);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    PBCPath() {}

    PBCDPoint _endpoint;
    bool _arc;

public:
    PBCPath(PBCDPoint endpoint, bool arc = false);
    PBCPath(double endpointX, double endpointY, bool arc = false);
    PBCDPoint endpoint() const;
};

typedef boost::shared_ptr<PBCPath> PBCPathSP;

#endif // PBCPATH_H
