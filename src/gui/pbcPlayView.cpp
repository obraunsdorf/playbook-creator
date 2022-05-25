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

#include "dialogs/mainDialog.h"
#include "pbcController.h"
#include "models/pbcPlaybook.h"
#include "util/pbcStorage.h"
#include "util/pbcConfig.h"
#include "gui/pbcPlayerView.h"
#include "gui/pbcSettings.h"
#include "QGraphicsEllipseItem"
#include "pbcController.h"
#include "models/pbcPlaybook.h"
#include "dialogs/pbcEditCategoriesDialog.h"
#include "util/pbcPositionTranslator.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

#include <string>
#include <iostream>
#include <dialogs/pbcSetPasswordDialog.h>

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
    _lastControlPoint.setX(DUMMY_POINT.get<0>());
    _lastControlPoint.setY(DUMMY_POINT.get<1>());
    repaint();
}

/**
 * @brief Displays the code name of the current play
 * @param yPos The vertical position of the line in coordinates of the
 * PBCPlayView instance (pixels)
 * @param textHeight The size of the displayed name
 * @param color The color of the displayed name
 */
void PBCPlayView::paintPlayName() {
    unsigned int textHeight = PBCConfig::getInstance()->playNameSize();
    QFont font = QFont(QString::fromStdString(PBCConfig::getInstance()->playNameFont()),
                       textHeight,
                       textHeight,
                       true);

    const QString name =
            _currentPlay->codeName() != "" ?
            QString::fromStdString(_currentPlay->codeName()) :
            QString::fromStdString(_currentPlay->name());


    // adjust font size for name to fit in one line
    unsigned int rightMargin = 5; // in pixel; so that play name is not squished against right border of canvas
    unsigned int leftMargin = 5; // in pixel; so that play name is not squished against left border of canvas
    bool fits = false;
    while (!fits) {
        font.setPointSize(textHeight);
        font.setWeight(textHeight);
        QFontMetrics fm(font);
        if (fm.width(name) >= PBCConfig::getInstance()->canvasWidth() - leftMargin - rightMargin) {
            fits = false;
            textHeight = textHeight - 1;
        } else {
            fits = true;
        }
    }

    QGraphicsTextItem *text = this->addText(name, font);
    unsigned int yPos = PBCConfig::getInstance()->canvasHeight() - 2 * textHeight;
    PBCColor color = PBCConfig::getInstance()->playNameColor();
    text->setY(yPos);
    text->setX(leftMargin);
    text->setDefaultTextColor(QColor(color.r(), color.g(), color.b()));

    /*
    // set text editable: could be a usable feature for the future
    text->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard | Qt::TextEditable);*/
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

    paintLine(PBCConfig::getInstance()->tenYdY(),
              PBCConfig::getInstance()->canvasWidth(),
              PBCConfig::getInstance()->fiveYdWidth(),
              PBCConfig::getInstance()->fiveYdColor());


    /*paintBall(PBCConfig::getInstance()->canvasWidth() / 2,
              PBCConfig::getInstance()->losY());*/

    paintBorder();

    if (_currentPlay != NULL) {
        for (PBCPlayerSP playerSP : *(_currentPlay->formation())) {
            this->addItem(new PBCPlayerView(playerSP, this));
        }

        unsigned int textHeight = PBCConfig::getInstance()->playNameSize();
        paintPlayName();
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
    PBCPlaySP play(new PBCPlay(name, codeName, formationName));
    _currentPlay = play;
    setActivePlay(_currentPlay);
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
void PBCPlayView::renameAndSavePlay(const std::string& name,
                       const std::string& codeName) {
    // FIXME(obr): dirty hack to check if the play can be rendered (no PBCRenderingException occurs)
    repaint();

    if (!_currentPlay)
        return;

    PBCController::getInstance()->getPlaybook()->deletePlay(_currentPlay->name());
    _currentPlay->setName(name);
    _currentPlay->setCodeName(codeName);
    PBCController::getInstance()->getPlaybook()->addPlay(_currentPlay, true);
    showPlay(_currentPlay->name());
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
    // FIXME(obr): dirty hack to check if the play can be rendered (no PBCRenderingException occurs)
    repaint();

    if(name != "") {
        _currentPlay->setName(name);
        _currentPlay->setCodeName(codeName);
    }
    PBCController::getInstance()->getPlaybook()->addPlay(_currentPlay, true);
    showPlay(_currentPlay->name());
}


/**
 * @brief Copies a play from the playbook and displays it
 * @param name The name of the play
 */
void PBCPlayView::showPlay(const std::string& name) {
    PBCPlaySP play = PBCController::getInstance()->getPlaybook()->getPlay(name);
    pbcAssert(play != NULL);
    _currentPlay.reset(new PBCPlay(*play));
    setActivePlay(_currentPlay);
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
    PBCController::getInstance()->getPlaybook()->addFormation(_currentPlay->formation(), true);
}


/**
 * @brief edits the current play's category assignment
 *
 * Opens a dialog for assigning categories to the current play and creating new ones.
 */
void PBCPlayView::editCategories() {
    savePlay();
    PBCPlaySP originalPlay = PBCController::getInstance()->getPlaybook()->getPlay(_currentPlay->name());  // NOLINT
    PBCEditCategoriesDialog dialog(originalPlay);
    dialog.editCategories();
    showPlay(originalPlay->name());
}

void debug_point(PBCDPoint point, const std::string msg) {
    std::cout << msg << " " << "x: " << point.get<0>() << " y: " << point.get<1>() << std::endl;
}


PBCDPoint playerPos_AfterMotion_inPixel(const PBCPlayerSP &playerSP) {
    int inOutFactor = -1;
    PBCDPoint playerPos = PBCPositionTranslator::getInstance()->translatePos(playerSP->pos());
    //debug_point(playerPos, "playerPos");
    if(playerPos.get<0>() < PBCConfig::getInstance()->canvasWidth() / 2) {
        inOutFactor = 1;
    }

    PBCDPoint afterMotionPos;
    PBCMotionSP motionSP = playerSP->motion();
    if (motionSP != NULL) {
        PBCDPoint afterMotionPos_inYd = PBCDPoint(playerSP->pos().get<0>() + inOutFactor*motionSP->motionEndPoint().get<0>(),
                                                  playerSP->pos().get<1>() + motionSP->motionEndPoint().get<1>());
        //debug_point(afterMotionPos_inYd, "afterMotion in Yd");
        afterMotionPos = PBCPositionTranslator::translatePos(afterMotionPos_inYd);
    } else {
        afterMotionPos = playerPos;
    }

    return afterMotionPos;
}

void PBCPlayView::enterRouteEditMode(PBCPlayerSP playerSP, RouteType routeType, const std::string& routeName, const std::string& routeCodeName, bool overwrite) {
    _routeEditMode = true;
    _routeType = routeType;
    _lastLine = NULL;
    _paths.clear();
    _routePlayer = playerSP;
    _routeName = routeName;
    _routeCodeName = routeCodeName;
    _overwrite = overwrite;

    switch(_routeType) {
        case RouteType::Route:
            _routePlayer->resetRoute();
            break;
        case RouteType::Alternative1:
            _routePlayer->resetAlternativeRoute(1);
            break;
        case RouteType::Alternative2:
            _routePlayer->resetAlternativeRoute(2);
            break;
        case RouteType::OptionRoute:
            break;
    }
    repaint();

    PBCDPoint afterMotionPos = playerPos_AfterMotion_inPixel(_routePlayer);

    QPointF startPoint(afterMotionPos.get<0>(), afterMotionPos.get<1>());
    _routeStartPos = startPoint;
    _lastPressPoint = _routeStartPos;

}

void PBCPlayView::enterMotionEditMode(PBCPlayerSP playerSP) {
    _motionEditMode = true;
    _lastLine = NULL;
    _paths.clear();
    _routePlayer = playerSP;

    std::vector<PBCPathSP> emptyRoutePaths;
    PBCMotionSP emptyMotion(new PBCMotion(emptyRoutePaths));
    PBCRouteSP emptyRoute = PBCRouteSP(new PBCRoute("empty", "", emptyRoutePaths));
    _routePlayer->setRoute(emptyRoute);
    _routePlayer->setMotion(emptyMotion);
    repaint();

    PBCDPoint translatedPos = PBCPositionTranslator::translatePos(_routePlayer->pos());
    QPointF startPoint(translatedPos.get<0>(), translatedPos.get<1>());
    _routeStartPos = startPoint;
    _lastPressPoint = _routeStartPos;

}

void PBCPlayView::leaveRouteMotionEditMode() {
    _routeEditMode = false;
    //_routeType = RouteType::Route;
    _motionEditMode = false;
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
    if (_routeEditMode == false && _motionEditMode == false) {
        return PBCGridIronView::mouseMoveEvent(event);
    }
    unsigned int newX = event->scenePos().x();
    unsigned int newY = event->scenePos().y();

    QPainterPath path;
    path.moveTo(_lastPressPoint);
    //if (event->buttons() & Qt::MouseButton::LeftButton) {
    if (QApplication::keyboardModifiers() & Qt::KeyboardModifier::ControlModifier) {
        qreal endX = _lastLine->path().currentPosition().x();
        qreal endY = _lastLine->path().currentPosition().y();

        this->removeItem(_lastLine);
        path.quadTo(QPointF(newX,newY), QPointF(endX, endY));
        _lastControlPoint.setX(newX);
        _lastControlPoint.setY(newY);
    } else {
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
    if ((_routeEditMode == false && _motionEditMode == false) || _lastLine == NULL) {
        return PBCGridIronView::mouseReleaseEvent(event);
    }
    unsigned int newX = _lastLine->path().currentPosition().x();
    unsigned int newY = _lastLine->path().currentPosition().y();

    /*int inOutFactor = -1;
    PBCDPoint playerPos = playerPos_AfterMotion_inPixel(_routePlayer);
    if(playerPos.get<0>() < PBCConfig::getInstance()->canvasWidth() / 2) {
        inOutFactor = 1;
    }*/

    int inOutFactor = -1;
    PBCDPoint afterMotionPos = playerPos_AfterMotion_inPixel(_routePlayer);
    if(afterMotionPos.get<0>() < PBCConfig::getInstance()->canvasWidth() / 2) {
        inOutFactor = 1;
    }


    PBCDPoint pathPoint =
            PBCPositionTranslator::getInstance()->retranslatePos(
                    PBCDPoint(newX,newY),
                    PBCDPoint(_routeStartPos.x(), _routeStartPos.y()));  // NOLINT
    PBCDPoint inOut_corrected_pathPoint(pathPoint.get<0>()*inOutFactor, pathPoint.get<1>());


    if (_lastControlPoint.x() != DUMMY_POINT.get<0>() && _lastControlPoint.x() != DUMMY_POINT.get<0>()) {
        PBCDPoint pathControlPoint =
                PBCPositionTranslator::getInstance()->retranslatePos(
                        PBCDPoint(_lastControlPoint.x(),_lastControlPoint.y()),
                        PBCDPoint(_routeStartPos.x(), _routeStartPos.y()));  // NOLINT
        PBCDPoint inOut_corrected_pathControlPoint(pathControlPoint.get<0>()*inOutFactor, pathControlPoint.get<1>());
        _lastControlPoint.setX(DUMMY_POINT.get<0>());
        _lastControlPoint.setY(DUMMY_POINT.get<1>());

        _paths.push_back(PBCPathSP(new PBCPath(inOut_corrected_pathPoint, inOut_corrected_pathControlPoint)));
    } else {
        _paths.push_back(PBCPathSP(new PBCPath(inOut_corrected_pathPoint)));
    }


    this->addItem(new QGraphicsPathItem(_lastLine->path()));
    _lastPressPoint.setX(newX);
    _lastPressPoint.setY(newY);
}

void PBCPlayView::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (_routeEditMode == true) {
        pbcAssert(_routePlayer);
        PBCRouteSP route(new PBCRoute(_routeName, _routeCodeName, _paths));
        if (_routeName != "") {
            try {
                PBCController::getInstance()->getPlaybook()->addRoute(route, _overwrite);
            } catch (PBCAutoSaveException& e) {
                QMessageBox::information(NULL, "", "You have to save the playbook to a file before you can add routes");  //NOLINT
                savePlaybookOnRouteCreation();
            }
        }
        switch(_routeType) {
            case RouteType::Route:
                _routePlayer->setRoute(route);
                break;
            case RouteType::OptionRoute:
                _routePlayer->addOptionRoute(route);
                break;
            case RouteType::Alternative1:
                _routePlayer->setAlternativeRoute(1, route);
                break;
            case RouteType::Alternative2:
                _routePlayer->setAlternativeRoute(2, route);
                break;
        }
        leaveRouteMotionEditMode();
        repaint();
    } else if (_motionEditMode == true) {
        pbcAssert(_routePlayer);
        PBCMotionSP motion(new PBCMotion(_paths));
        _routePlayer->setMotion(motion);
        leaveRouteMotionEditMode();
        repaint();
    } else {
        PBCGridIronView::mouseDoubleClickEvent(event);
    }
}

void PBCPlayView::savePlaybookOnRouteCreation() {
    std::string stdFile = PBCController::getInstance()->getPlaybook()->name() + ".pbc";
    QFileDialog fileDialog(
            NULL, "Save Playbook",
            getLastPlaybookLocation(QString::fromStdString(stdFile)),
            "PBC Files (*.pbc);;All Files (*.*)");

    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    if(fileDialog.exec() == true) {
        QStringList files = fileDialog.selectedFiles();
        pbcAssert(files.size() == 1);
        QString fileName = files.first();

        PBCSetPasswordDialog pwDialog;
        int returnCode = pwDialog.exec();
        if (returnCode == QDialog::Accepted) {
            QString password = pwDialog.getPassword();
            PBCStorage::getInstance()->savePlaybook(password.toStdString(),
                                                    fileName.toStdString());
            setLastPlaybookLocation(QFileInfo(fileName));
        }
    }
}

void PBCPlayView::setActivePlay(PBCPlaySP playSP) {
    MainDialog* mainDialog = dynamic_cast<MainDialog*>(this->parent());
    if (mainDialog != NULL) {
        mainDialog->fillPlayInfoDock(playSP);
    }
}

void PBCPlayView::setActivePlayer(PBCPlayerSP playerSP) {
    MainDialog* mainDialog = dynamic_cast<MainDialog*>(this->parent());
    _activePlayer = playerSP;
    if (mainDialog != NULL) {
        mainDialog->fillPlayerInfoDock(playerSP);
    }
}

void PBCPlayView::setActivePlayerColor(PBCColor color) {
    if(_activePlayer != NULL) {
        _activePlayer->setColor(color);
        repaint();
    }
}

void PBCPlayView::setActivePlayerRoute(PBCRouteSP route) {
    if(_activePlayer != NULL) {
       _activePlayer->setRoute(route);
       repaint();
    }
}

void PBCPlayView::setActivePlayerName(std::string name) {
    if(_activePlayer != NULL) {
        _activePlayer->setName(name);
        repaint();
    }
}

void PBCPlayView::setActivePlayerNr(unsigned int nr) {
    if(_activePlayer != NULL) {
        _activePlayer->setNr(nr);
        repaint();
    }
}

void PBCPlayView::setPlayComment(const std::string &comment) {
    if(_currentPlay != NULL) {
        _currentPlay->setComment(comment);
    }
}



