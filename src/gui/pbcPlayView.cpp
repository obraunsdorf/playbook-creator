/** @file pbcPlayView.cpp
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

#include "pbcPlayView.h"

#include "util/pbcConfig.h"
#include "gui/pbcPlayerView.h"
#include "QGraphicsEllipseItem"
#include "models/pbcPlaybook.h"
#include "dialogs/pbcEditCategoriesDialog.h"
#include "util/pbcPositionTranslator.h"
#include <QApplication>

#include <string>

/**
 * @class PBCPlayView
 * @brief A subclass of PBCGridIronView to display a play from the playbook
 * inside the main dialog.
 */

/**
 * @brief The constructor.
 * @param playSP a smart pointer to a PBCPlay instance that should be displayed
 * @param parent The parent object this dialog belongs to.
 */
PBCPlayView::PBCPlayView(PBCPlaySP playSP, QObject *parent) :
    PBCGridIronView(parent),
    _currentPlay(playSP) {
    repaint();
}

/**
 * @brief Displays the code name of the current play
 * @param yPos The vertical position of the line in coordinates of the
 * PBCPlayView instance (pixels)
 * @param textSize The size of the displayed name
 * @param color The color of the displayed name
 */
void PBCPlayView::paintPlayName(unsigned int yPos,
                                unsigned int textSize,
                                PBCColor color) {
    QGraphicsTextItem* text = this->addText(QString::fromStdString(
                                                _currentPlay->codeName()),
                                            QFont("Helvetica",
                                                  textSize,
                                                  textSize,
                                                  true));
    text->setY(yPos);
    text->setDefaultTextColor(QColor(color.r(), color.g(), color.b()));
}


/**
 * @brief Paints grid iron and the current play on it
 */
void PBCPlayView::repaint() {
    this->clear();
    paintLine(PBCConfig::getInstance()->losY(),
              PBCConfig::getInstance()->canvasWidth(),
              PBCConfig::getInstance()->losWidth(),
              PBCConfig::getInstance()->losColor());

    paintLine(PBCConfig::getInstance()->fiveYdY(),
              PBCConfig::getInstance()->canvasWidth(),
              PBCConfig::getInstance()->fiveYdWidth(),
              PBCConfig::getInstance()->fiveYdColor());

    paintBall(PBCConfig::getInstance()->canvasWidth() / 2,
              PBCConfig::getInstance()->losY());

    paintBorder();

    if(_currentPlay != NULL) {
        for(PBCPlayerSP playerSP : *(_currentPlay->formation())) {
            this->addItem(new PBCPlayerView(playerSP, this));
        }

        unsigned int textSize = PBCConfig::getInstance()->playNameSize();
        paintPlayName(PBCConfig::getInstance()->canvasHeight() - 2 * textSize,
                      textSize,
                      PBCConfig::getInstance()->playNameColor());
    }
}

/**
 * @brief Resets the current play and paints it.
 *
 * This results in an empty grid iron
 */
void PBCPlayView::resetPlay() {
    _currentPlay = NULL;
    repaint();
}


/**
 * @brief Creates a new play with the given formation
 * on the grid iron and sets it as the current play
 * @param name The name of the play
 * @param codeName The code name of the play
 * @param formationName The name of the play's formation
 */
void PBCPlayView::createNewPlay(const std::string &name,
                                const std::string &codeName,
                                const std::string &formationName) {
    PBCPlaySP play(new PBCPlay(name, codeName));
    if(formationName != "") {
        PBCFormationSP formation = PBCPlaybook::getInstance()->getFormation(formationName);  // NOLINT
        assert(formation != NULL);
        play->setFormation(formation);
    }
    _currentPlay = play;
    repaint();
}


/**
 * @brief Adds the current play to the playbook
 *
 * If a name is specified, the current play is saved with the given name and
 * code name. Otherwise it is saved with its original name.
 * @param name The new name of the play
 * @param codeName The new code name of the play
 */
void PBCPlayView::savePlay(const std::string &name,
                           const std::string &codeName) {
    if(name != "") {
        _currentPlay->setName(name);
        _currentPlay->setCodeName(codeName);
    }
    PBCPlaybook::getInstance()->addPlay(_currentPlay, true);
    showPlay(_currentPlay->name());
}


/**
 * @brief Copies a play from the playbook and displays it
 * @param name The name of the play
 */
void PBCPlayView::showPlay(const std::string& name) {
    PBCPlaySP play = PBCPlaybook::getInstance()->getPlay(name);
    assert(play != NULL);
    _currentPlay.reset(new PBCPlay(*play));
    repaint();
}

/**
 * @brief saves the (maybe modified) formation of the
 * current play to the playbook.
 *
 * If a formation name is specified, the current formation is saved with the
 * given name. Otherwise it is saved with its original name.
 * @param formationName The new name of the formation
 */
void PBCPlayView::saveFormation(const std::string &formationName) {
    if(formationName != "") {
        _currentPlay->formation()->setName(formationName);
    }
    PBCPlaybook::getInstance()->addFormation(_currentPlay->formation());
}


/**
 * @brief edits the current play's category assignment
 *
 * Opens a dialog for assigning categories to the current play and creating new ones.
 */
void PBCPlayView::editCategories() {
    savePlay();
    PBCPlaySP originalPlay = PBCPlaybook::getInstance()->getPlay(_currentPlay->name());  // NOLINT
    PBCEditCategoriesDialog dialog(originalPlay);
    dialog.editCategories();
    showPlay(originalPlay->name());
}


void PBCPlayView::enterRouteEditMode(PBCPlayerSP playerSP) {
    _routeEditMode = true;
    _lastLine = NULL;
    _paths.clear();
    _routePlayer = playerSP;

    std::vector<PBCPathSP> emptyRoutePaths;
    PBCRouteSP emptyRoute = PBCRouteSP(new PBCRoute("empty", "", emptyRoutePaths));
    _routePlayer->setRoute(emptyRoute);
    repaint();

    PBCDPoint translatedPos = PBCPositionTranslator::translatePos(_routePlayer->pos());
    QPointF startPoint(translatedPos.get<0>(), translatedPos.get<1>());
    _routeStartPos = startPoint;
    _lastPressPoint = _routeStartPos;

}

void PBCPlayView::leaveRouteEditMode() {
    _routeEditMode = false;
}

/**
 * @brief A Qt event handler that is
 * triggered whe the mouse moves over the PBCCustomRouteView instance
 *
 * Paints a new line from the endpoint of the current route to the mouse
 * position
 * @param event Contains mouse event data (the position of the mouse)
 */
void PBCPlayView::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (_routeEditMode == false) {
        return PBCGridIronView::mouseMoveEvent(event);
    }
    unsigned int newX = event->scenePos().x();
    unsigned int newY = event->scenePos().y();

    QPainterPath path;
    //if (event->buttons() & Qt::MouseButton::LeftButton) {
    if (QApplication::keyboardModifiers() & Qt::KeyboardModifier::ControlModifier) {
        qreal startX = _lastPressPoint.x();
        qreal startY = _lastPressPoint.y();
        qreal endX = _lastLine->path().currentPosition().x();
        qreal endY = _lastLine->path().currentPosition().y();

        this->removeItem(_lastLine);
        path.moveTo(startX, startY);
        path.quadTo(QPointF(newX,newY), QPointF(endX, endY));
        _lastControlPoint.setX(newX);
        _lastControlPoint.setY(newY);
    } else {
        path.moveTo(_lastPressPoint.x(), _lastPressPoint.y());
        path.lineTo(newX, newY);
        if(_lastLine != NULL) {
            this->removeItem(_lastLine);
            delete _lastLine;
            _lastLine = NULL;
        }
    }

    _lastLine = new QGraphicsPathItem(path);
    this->addItem(_lastLine);
}


/**
 * @brief A Qt event handler that is
 * triggered whe the mouse is clicked over the PBCCustomRouteView instance
 *
 * This will add the line between the endpoint of the current route and the
 * mouse position to the _paths vector
 * @param event Contains mouse event data (the position of the mouse)
 */
void PBCPlayView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (_routeEditMode == false) {
        return PBCGridIronView::mouseReleaseEvent(event);
    }
    unsigned int newX = _lastLine->path().currentPosition().x();
    unsigned int newY = _lastLine->path().currentPosition().y();

    int inOutFactor = -1;
    PBCDPoint playerPos = PBCPositionTranslator::getInstance()->translatePos(_routePlayer->pos());
    if(playerPos.get<0>() < PBCConfig::getInstance()->canvasWidth() / 2) {
        inOutFactor = 1;
    }
    PBCDPoint pathPoint =
            PBCPositionTranslator::getInstance()->retranslatePos(
                    PBCDPoint(newX,newY),
                    PBCDPoint(_routeStartPos.x(), _routeStartPos.y()));  // NOLINT
    PBCDPoint inOut_corrected_pathPoint(pathPoint.get<0>()*inOutFactor, pathPoint.get<1>());
    _paths.push_back(PBCPathSP(new PBCPath(inOut_corrected_pathPoint)));
    this->addItem(new QGraphicsPathItem(_lastLine->path()));
    _lastPressPoint.setX(newX);
    _lastPressPoint.setY(newY);
}

void PBCPlayView::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (_routeEditMode == false) {
        return PBCGridIronView::mouseDoubleClickEvent(event);
    }
    leaveRouteEditMode();
    PBCRouteSP route(new PBCRoute("_TEMP_", "", _paths));
    assert(_routePlayer);
    _routePlayer->setRoute(route);
    repaint();
}


