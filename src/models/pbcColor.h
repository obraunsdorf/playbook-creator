/** @file pbcColor.h
    This file is part of Playbook Creator.

    Playbook Creator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Playbook Creator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Playbook Creator.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2015 Oliver Braunsdorf

    @author Oliver Braunsdorf
*/

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

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {  // NOLINT
        assert(version == 0);
        ar & _r;
        ar & _g;
        ar & _b;
    }

 public:
    PBCColor();
    PBCColor(unsigned int r, unsigned int g, unsigned int b);
    PBCColor(const PBCColor& obj);

    unsigned int r() const;
    unsigned int g() const;
    unsigned int b() const;
};

#endif  // PBCCOLOR_H
