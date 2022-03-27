/** @file pbcPlayerView.h
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

#ifndef PBCPLAYERVIEW_H
#define PBCPLAYERVIEW_H

#include <QGraphicsItemGroup>
#include <QGraphicsSceneContextMenuEvent>
#include <boost/shared_ptr.hpp>
#include "models/pbcPlayer.h"
#include "util/pbcDeclarations.h"
#include "gui/pbcPlayView.h"
#include <boost/unordered/unordered_map.hpp>
#include "QAction"
#include <vector>


#define ACTION_TEXT_RESET "Reset"
#define ACTION_TEXT_NAMED_ROUTE "Create (named) route"
#define ACTION_TEXT_UNNAMED_ROUTE "Create route (quick, unnamed)"

class PBCPlayerView;
typedef boost::shared_ptr<PBCPlayerView> PBCPlayerViewSP;
typedef boost::shared_ptr<QGraphicsItem> QGraphicsItemSP;
typedef boost::unordered_map<QAction*, PBCRouteSP> PBCRouteActionMap;


class PBCPlayerView : public QObject, public QGraphicsItemGroup {
    Q_OBJECT
 public:
    explicit PBCPlayerView(PBCPlayerSP playerSP, PBCPlayView* playView);

 private:
    PBCPlayerSP _playerSP;
    PBCPlayView* _playView;
    PBCDPoint _originalPos;
    boost::shared_ptr<QAbstractGraphicsShapeItem> _playerShapeSP;
    std::vector<boost::shared_ptr<QGraphicsItem>> _routePaths;
    std::vector<boost::shared_ptr<QGraphicsItem>> _motionPaths;

    void repaint();
    void paintRoutes();
    void __paintRoutes(PBCRouteSP route, RouteType mode);
    bool isClickInShape(const QPointF& clickPos);
    void createNamedRoute(RouteType routeType);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void joinPaths(const std::vector<PBCPathSP>& paths,
                   std::vector<QGraphicsItemSP>* graphicItems,
                   PBCDPoint basePoint,
                   RouteType routetype);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void applyMotion(PBCMotionSP motion);
    void setColor(PBCColor color);
    void setPosition(double x, double y);
};

#endif  // PBCPLAYERVIEW_H
