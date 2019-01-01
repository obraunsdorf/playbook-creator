/** @file PBCKnob.cpp
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

#include "pbcPathView.h"

#include "util/pbcDeclarations.h"
#include "util/pbcConfig.h"
#include <QPainter>
#include <QtWidgets/QGraphicsEllipseItem>

PBCPathView::PBCPathView(QPointF start, QPointF end, double width, bool joinAtStart) :
    _start(start),
    _end(end),
    _width(width){
    // see https://stackov erflow.com/questions/7854043/drawing-rectangle-between-two-points-with-arbitrary-width
    QPointF vector(_end.x() - start.x(), _end.y() - start.y());
    double length = std::sqrt(
            std::pow(vector.x(), 2.0)
            +
            std::pow(vector.y(), 2.0));

    _normalizedVector = QPointF(vector.x()/length, vector.y()/length);
    _perpendicularVector = QPointF(_normalizedVector.y(), -_normalizedVector.x());

    QPointF widthVector(_perpendicularVector * width/2);

    QPointF a = start - widthVector;
    QPointF b = start + widthVector;
    QPointF c = _end + widthVector;
    QPointF d = _end - widthVector;


    QPainterPath path(a);
    if (joinAtStart) {
        path.quadTo(_start - _normalizedVector*_width/2, b);
    } else {
        path.lineTo(b);
    }
    path.lineTo(c);
    path.quadTo(_end + _normalizedVector*_width/2, d);
    path.lineTo(a);
    this->setPath(path);
}
