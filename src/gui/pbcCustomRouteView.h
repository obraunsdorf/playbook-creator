/** @file pbcCustomRouteView.h
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

#ifndef PBCCUSTOMROUTEVIEW_H
#define PBCCUSTOMROUTEVIEW_H

#include "gui/pbcGridIronView.h"
#include "models/pbcPath.h"
#include "models/pbcRoute.h"
#include <QGraphicsLineItem>
#include <string>
#include <vector>

class PBCCustomRouteView : public PBCGridIronView {
 public:
    explicit PBCCustomRouteView(QObject *parent = 0);
    PBCRouteSP createRoute(const std::string& name,
                           const std::string& codeName);

 protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

 private:
    QGraphicsLineItem* _lastLine;
    QPointF _routeStartPos;
    QPointF _lastPressPoint;
    std::vector<PBCPathSP> _paths;
};

#endif  // PBCCUSTOMROUTEVIEW_H
