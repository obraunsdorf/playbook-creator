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
#include <vector>

class PBCPlayerView;
typedef boost::shared_ptr<PBCPlayerView> PBCPlayerViewSP;
typedef boost::shared_ptr<QGraphicsItem> QGraphicsItemSP;

class PBCPlayerView : public QObject, public QGraphicsItemGroup {
    Q_OBJECT
 public:
    explicit PBCPlayerView(PBCPlayerSP playerSP);

 private:
    PBCPlayerSP _playerSP;
    PBCDPoint _originalPos;
    boost::shared_ptr<QAbstractGraphicsShapeItem> _playerShapeSP;
    std::vector<boost::shared_ptr<QGraphicsItem>> _routePaths;
    std::vector<boost::shared_ptr<QGraphicsItem>> _motionPaths;
    boost::shared_ptr<QGraphicsPathItem> _joinedRouteItem;

    QColor _shapeColor;
    QColor _contourColor;

    void repaint();
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void joinPaths(const std::vector<PBCPathSP>& paths,
                   std::vector<QGraphicsItemSP>* graphicItems,
                   PBCDPoint basePoint);
    void applyRoute(PBCRouteSP route);
    void applyMotion(PBCMotionSP motion);
    void setColor(PBCColor color);
    void setPosition(double x, double y);
};

#endif  // PBCPLAYERVIEW_H
