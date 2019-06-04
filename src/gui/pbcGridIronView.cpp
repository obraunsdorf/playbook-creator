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


/**
 * @class PBCGridIronView
 * @brief A canvas class that paints a grid iron on it.
 *
 * This class extends a QGraphicsscene. An instance of this class can be seen as
 * a canvas on which it paints a grid iron itself. In this context a grid iron
 * is an american football field, consisting of a few lines (line of scrimmage,
 * 5yd marking line, 10yd marking line,...) and a ball (the football)
 */

/**
 * @brief The constructor
 * @param parent The parent object this dialog belongs to.
 */
PBCGridIronView::PBCGridIronView(QObject *parent) :
    QGraphicsScene(parent) {}

/**
 * @brief Paints a line.
 * @param yPos The vertical position of the line in coordinates of the
 * PBCGridIronView instance (pixels)
 * @param width The horizontal stretch of the line in pixels
 * @param lineWidth The width of the line
 * @param color The color of the line
 */
void PBCGridIronView::paintLine(unsigned int yPos,
                                unsigned int width,
                                unsigned int lineWidth,
                                PBCColor color) {
    QPen pen(QColor(color.r(), color.g(), color.b()));
    pen.setWidth(lineWidth);
    this->addLine(0+lineWidth/2, yPos, width-lineWidth/2, yPos, pen);
}


/**
 * @brief Paints the ball.
 * @param xPos The horizontal position of the ball in coordinates of the
 * PBCGridIronView instance (pixels)
 * @param yPos The vertical position of the ball in coordinates of the
 * PBCGridIronView instance (pixels)
 * @param zValue The 3D z-value of the ball. Used to render the ball on top of
 * the PBCPlayerView instances or below
 */
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

/**
 * @brief Paints the border lines of the grid iron.
 */
void PBCGridIronView::paintBorder() {
    this->addRect(0,
                  0,
                  PBCConfig::getInstance()->canvasWidth(),
                  PBCConfig::getInstance()->canvasHeight());
}
