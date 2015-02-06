#ifndef PBCDECLARATIONS_H
#define PBCDECLARATIONS_H

#include <string>
#include <assert.h>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/geometry.hpp>

struct PBCRole {
    std::string fullName;
    char shortName[3];
};

class PBCColor {
private:
    unsigned int r;
    unsigned int g;
    unsigned int b;

public:
    PBCColor() {}

    PBCColor(unsigned int& r, unsigned int& g, unsigned int& b) {
        assert(r <= 255);
        assert(g <= 255);
        assert(b <= 255);
        this->r = r;
        this->g = g;
        this->b = b;
    }

    PBCColor(const PBCColor& obj) {
        std::vector<unsigned int> c = obj.getColor();
        this->r = c.at(0);
        this->g = c.at(1);
        this->b = c.at(2);
    }

    std::vector<unsigned int> getColor() const {
        std::vector<unsigned int> color;
        color.push_back(r);
        color.push_back(g);
        color.push_back(b);
        return color;
    }
};

typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> PBCDPoint;


#endif // PBCDECLARATIONS_H
