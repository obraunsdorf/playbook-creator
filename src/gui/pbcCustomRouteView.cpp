/** @file pbcCustomRouteView.cpp
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

#include "pbcCustomRouteView.h"
#include "util/pbcConfig.h"
#include "util/pbcPositionTranslator.h"
#include "pbcController.h"
#include "models/pbcPlaybook.h"
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <string>

/**
 * @class PBCCustomRouteView
 * @brief A subclass of PBCGridIronView to display a grid iron and a custom
 * route that has been created
 */

/**
 * @brief The constructor
 * @param parent The parent object this dialog belongs to.
 */
PBCCustomRouteView::PBCCustomRouteView(QObject *parent) :
    PBCGridIronView(parent),
    _lastLine(NULL) {
    unsigned int height = 15 * PBCConfig::getInstance()->ydInPixel();
    unsigned int width = 10 * PBCConfig::getInstance()->ydInPixel();
    this->setSceneRect(0, 0, width, height);

    unsigned int losY = height - 2 * PBCConfig::getInstance()->ydInPixel();
    unsigned int fiveYdY = losY - 5 * PBCConfig::getInstance()->ydInPixel();
    unsigned int tenYdY = losY - 10 * PBCConfig::getInstance()->ydInPixel();
    paintLine(losY,
              width,
              PBCConfig::getInstance()->losWidth(),
              PBCConfig::getInstance()->losColor());

    paintLine(fiveYdY,
              width,
              PBCConfig::getInstance()->fiveYdWidth(),
              PBCConfig::getInstance()->losColor());

    paintLine(tenYdY,
              width,
              PBCConfig::getInstance()->fiveYdWidth(),
              PBCConfig::getInstance()->losColor());

    paintBall(width - PBCConfig::getInstance()->ballWidth(), losY);

    // Player
    this->addEllipse(width / 2 - 10, losY, 20, 20);
    _routeStartPos = QPointF(this->width() / 2, losY);
    _lastPressPoint = _routeStartPos;
}

/**
 * @brief Creates a new route from the painted
 * paths
 * @param name The name of the new route
 * @param codeName The code name of the new route
 * @return a smart pointer to the new route
 */
PBCRouteSP PBCCustomRouteView::createRoute(const std::string &name,
                                           const std::string &codeName) {
    PBCRouteSP route(new PBCRoute(name, codeName, _paths));
    return route;
}


/**
 * @brief A Qt event handler that is
 * triggered whe the mouse moves over the PBCCustomRouteView instance
 *
 * Paints a new line from the endpoint of the current route to the mouse
 * position
 * @param event Contains mouse event data (the position of the mouse)
 */
void PBCCustomRouteView::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(_lastLine != NULL) {
        this->removeItem(_lastLine);
        delete _lastLine;
        _lastLine = NULL;
    }
    unsigned int newX = event->scenePos().x();
    unsigned int newY = event->scenePos().y();
    _lastLine = this->addLine(_lastPressPoint.x(),
                              _lastPressPoint.y(),
                              newX,
                              newY);
}


/**
 * @brief A Qt event handler that is
 * triggered whe the mouse is clicked over the PBCCustomRouteView instance
 *
 * This will add the line between the endpoint of the current route and the
 * mouse position to the _paths vector
 * @param event Contains mouse event data (the position of the mouse)
 */
void PBCCustomRouteView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    unsigned int newX = event->scenePos().x();
    unsigned int newY = event->scenePos().y();
    PBCDPoint pathPoint =
            PBCPositionTranslator::getInstance()->retranslatePos(PBCDPoint(newX, newY),                               // NOLINT
                                                                 PBCDPoint(_routeStartPos.x(), _routeStartPos.y()));  // NOLINT
    _paths.push_back(PBCPathSP(new PBCPath(pathPoint)));
    this->addLine(_lastPressPoint.x(), _lastPressPoint.y(), newX, newY);
    _lastPressPoint.setX(newX);
    _lastPressPoint.setY(newY);
}
