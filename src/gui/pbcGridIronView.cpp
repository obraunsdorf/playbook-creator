/** @file pbcGridIronView.cpp
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

#include "pbcGridIronView.h"
#include "util/pbcConfig.h"
#include <QGraphicsEllipseItem>
#include <vector>

PBCGridIronView::PBCGridIronView(QObject *parent) :
    QGraphicsScene(parent) {}

void PBCGridIronView::paintLine(unsigned int yPos,
                                unsigned int width,
                                unsigned int lineWidth,
                                PBCColor color) {
    QPen pen(QColor(color.r(), color.g(), color.b()));
    pen.setWidth(lineWidth);
    this->addLine(0, yPos, width - pen.width(), yPos, pen);
}

void PBCGridIronView::paintBall(unsigned int xPos,
                                unsigned int yPos,
                                unsigned int zValue) {
    PBCColor color = PBCConfig::getInstance()->ballColor();
    QColor ballColor(color.r(), color.g(), color.b());
    QPen ballPen(ballColor);
    QBrush ballBrush(ballColor);
    unsigned int ballWidth = PBCConfig::getInstance()->ballWidth();
    QGraphicsEllipseItem* ball = this->addEllipse(xPos - ballWidth / 2,
                                                  yPos - ballWidth, ballWidth,
                                                  2 * ballWidth,
                                                  ballPen,
                                                  ballBrush);
    ball->setZValue(zValue);
}

void PBCGridIronView::paintBorder() {
    this->addRect(0,
                  0,
                  PBCConfig::getInstance()->canvasWidth(),
                  PBCConfig::getInstance()->canvasHeight());
}
