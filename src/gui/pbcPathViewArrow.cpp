/** @file pbcArrow.cpp
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

    Copyright 2015-2018 Oliver Braunsdorf

    @author Oliver Braunsdorf
*/

#include "pbcPathViewArrow.h"

#include "util/pbcDeclarations.h"
#include "util/pbcConfig.h"
#include <QPainter>

PBCPathViewArrow::PBCPathViewArrow(QPointF start, QPointF end, double width, bool joinAtStart) :
    PBCPathView(start, end, width, joinAtStart)
    {
        QPointF widthVector(_perpendicularVector * width/2);
        QPointF arrowHeadStart = end - _normalizedVector*2*width;

        QPointF a(start - widthVector);
        QPointF b(start + widthVector);
        QPointF c(arrowHeadStart + widthVector);
        QPointF d(arrowHeadStart + 2*widthVector);
        QPointF e(end);
        QPointF f(arrowHeadStart - 2*widthVector);
        QPointF g(arrowHeadStart - widthVector);
        QPointF h(start - widthVector);

        QPainterPath path(a);
        if (joinAtStart) {
            path.quadTo(_start - _normalizedVector*_width/2, b);
        } else {
            path.lineTo(b);
        }
        path.lineTo(c);
        path.lineTo(d);
        path.lineTo(e);
        path.lineTo(f);
        path.lineTo(g);
        path.lineTo(h);
        this->setPath(path);
}
