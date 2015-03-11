#ifndef PBCCOLOR_H
#define PBCCOLOR_H

#include <vector>
#include <assert.h>
#include <boost/serialization/access.hpp>

class PBCColor {
friend class boost::serialization::access;
private:
    unsigned int _r;
    unsigned int _g;
    unsigned int _b;

    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        assert(version == 0);
        ar & _r;
        ar & _g;
        ar & _b;
    }

public:
    PBCColor();
    PBCColor(unsigned int r, unsigned int g, unsigned int b);
    PBCColor(const PBCColor& obj);

    std::vector<unsigned int> getColorVector() const;
    unsigned int r() const;
    unsigned int g() const;
    unsigned int b() const;
};

#endif // PBCCOLOR_H
