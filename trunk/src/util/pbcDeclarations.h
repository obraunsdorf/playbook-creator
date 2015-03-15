#ifndef PBCDECLARATIONS_H
#define PBCDECLARATIONS_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/geometry.hpp>
#include <boost/array.hpp>

struct PBCRole {
    std::string fullName;
    boost::array<char, 4> shortName;
};

using boost::geometry::model::point;
typedef point<double, 2, boost::geometry::cs::cartesian> PBCDPoint;


#endif  // PBCDECLARATIONS_H
