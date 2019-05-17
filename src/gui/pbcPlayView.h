/** @file pbcPlayView.h
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

#ifndef PBCPLAYVIEW_H
#define PBCPLAYVIEW_H

#include "gui/pbcGridIronView.h"
#include "models/pbcPlay.h"
#include <string>

class PBCPlayView : public PBCGridIronView {
 public:
    explicit PBCPlayView(PBCPlaySP playSP = NULL, QObject *parent = 0);
    void paintPlayName(unsigned int yPos,
                       unsigned int textSize,
                       PBCColor color);
    void repaint();
    void resetPlay();
    void createNewPlay(const std::string& name,
                       const std::string& codeName,
                       const std::string& formationName = "");
    void showPlay(const std::string &name);
    void savePlay(const std::string& name = "",
                  const std::string& codeName = "");
    void saveFormation(const std::string& formationName = "");
    void editCategories();

    void enterRouteEditMode(PBCPlayerSP playerSP);
    void leaveRouteEditMode();
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

 private:
    PBCPlaySP _currentPlay;

    bool _routeEditMode = false;
    PBCPlayerSP _routePlayer;
    QGraphicsLineItem* _lastLine;
    QPointF _routeStartPos;
    QPointF _lastPressPoint;
    std::vector<PBCPathSP> _paths;
};

#endif  // PBCPLAYVIEW_H
