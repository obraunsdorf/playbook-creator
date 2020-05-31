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
#include "models/pbcPlayer.h"
#include <string>

class PBCPlayView : public PBCGridIronView {
 public:
    explicit PBCPlayView(PBCPlaySP playSP = NULL, QObject *parent = 0);
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
    void setPlayComment(const std::string& comment);

    void savePlaybookOnRouteCreation();

    void enterRouteEditMode(
            PBCPlayerSP playerSP,
            bool optionRouteMode,
            const std::string& routeName = "",
            const std::string& routeCodeName = "",
            bool overwrite = false);
    void enterMotionEditMode(PBCPlayerSP playerSP);
    void leaveRouteMotionEditMode();
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void setActivePlay(PBCPlaySP playSP);
    void setActivePlayer(PBCPlayerSP playerSP);

    void setActivePlayerColor(PBCColor color);
    void setActivePlayerRoute(PBCRouteSP route);
    void setActivePlayerName(std::string name);
    void setActivePlayerNr(unsigned int nr);


 private:
    PBCPlaySP _currentPlay;
    PBCPlayerSP _activePlayer;

    bool _routeEditMode = false;
    bool _optionRouteMode = false;
    bool _motionEditMode = false;
    PBCPlayerSP _routePlayer;
    std::string _routeName;
    std::string _routeCodeName;
    bool _overwrite;
    QGraphicsPathItem* _lastLine;
    QPointF _routeStartPos;
    QPointF _lastPressPoint;
    QPointF _lastControlPoint;
    std::vector<PBCPathSP> _paths;

    void paintPlayName();
};

#endif  // PBCPLAYVIEW_H
