/** @file pbcPlayerView.cpp
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

#include "pbcPlayerView.h"

#include "pbcController.h"
#include "models/pbcPlaybook.h"
#include "util/pbcDeclarations.h"
#include "dialogs/pbcSavePlayAsDialog.h"
#include "QBrush"
#include "QMenu"
#include "util/pbcPositionTranslator.h"
#include "dialogs/pbcCustomRouteDialog.h"
#include <boost/unordered/unordered_map.hpp>
#include <utility>
#include "QDebug"
#include <vector>
#include <set>
#include <QMessageBox>
#include <QColorDialog>
#include <QInputDialog>
#include <QGraphicsPathItem>
#include <QGraphicsDropShadowEffect>
#include <iostream>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

/**
 * @class PBCPlayerView
 * @brief A graphical representation of a football player.
 *
 * This includes the player's "body", motion and route. They are all QGraphicItems
 * which are combined as an QGraphicsItemGroup. Instances of
 * PBCPlayerView are typically displayed in PBCPlayView and PBCCustomRouteDialog.
 */

/**
 * @brief The constructor.
 * @param playerSP The data model of the player to display
 */
PBCPlayerView::PBCPlayerView(PBCPlayerSP playerSP, PBCPlayView* playView) :
    _playerSP(playerSP),
    _playView(playView) {
    repaint();
}

/**
 * @brief Painting the player represented  by _playerSP
 */
void PBCPlayerView::repaint() {
    _originalPos = PBCPositionTranslator::getInstance()->translatePos(_playerSP->pos());  // NOLINT
    unsigned int playerWidth = PBCConfig::getInstance()->playerWidth();
    double playerPosX = _originalPos.get<0>() - playerWidth / 2;
    double playerPosY = _originalPos.get<1>();
    if (_playerSP->role().fullName == "Center") {
        _playerShapeSP.reset(new QGraphicsRectItem(playerPosX,
                                                   playerPosY,
                                                   playerWidth,
                                                   playerWidth));
    } else {
        _playerShapeSP.reset(new QGraphicsEllipseItem(playerPosX,
                                                      playerPosY,
                                                      playerWidth,

                                                      playerWidth));
    }

    PBCColor color = _playerSP->color();
    _playerShapeSP->setBrush(QBrush(QColor(color.r(), color.g(), color.b())));
    this->addToGroup(_playerShapeSP.get());
    this->setFlag(QGraphicsItem::ItemIsMovable);

    if(_playerSP->motion() != NULL) {
        applyMotion(_playerSP->motion());
    }
    if(_playerSP->route() != NULL) {
        applyRoute(_playerSP->route());
    }

    if (PBCConfig::getInstance()->playerShadow()) {
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(PBCConfig::getInstance()->playerShadowRadius());
        shadow->setOffset(PBCConfig::getInstance()->playerShadowOffset());
        setGraphicsEffect(shadow);
    }
}


/**
 * @brief Concatenates the lines of the motion and the
 * route of the player.
 *
 * I wrote that function some time ago and it confuses me now^^. I don't know
 * what the meaning of the parameters is..but it works ;)
 * @param paths
 * @param graphicItems
 * @param basePoint
 */
void PBCPlayerView::joinPaths(const std::vector<PBCPathSP>& paths,
                              std::vector<QGraphicsItemSP>* graphicItems,
                              PBCDPoint basePoint) {
    pbcAssert(graphicItems == &_routePaths || graphicItems == &_motionPaths);
    if (paths.empty()) {
        return;
    }
    PBCColor color = _playerSP->color();
    QBrush brush(QColor(color.r(), color.g(), color.b()));
    QPen pen(brush, PBCConfig::getInstance()->routeWidth(), Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap, Qt::PenJoinStyle::RoundJoin);
    if(graphicItems == &_motionPaths) {
        pen.setStyle(Qt::DashLine);
    }
    double baseX = basePoint.get<0>();
    double baseY = basePoint.get<1>();
    double lastX = baseX;
    double lastY = baseY;
    int inOutFactor = -1;
    if (basePoint.get<0>() < PBCConfig::getInstance()->canvasWidth() / 2) {
        inOutFactor = 1;
    }
    for(PBCPathSP path : paths) {
        PBCDPoint endPointYd(inOutFactor * path->endpoint().get<0>(),
                             path->endpoint().get<1>());
        PBCDPoint endPointPixel = PBCPositionTranslator::getInstance()->translatePos(endPointYd, basePoint); //NOLINT
        unsigned int endPointX = endPointPixel.get<0>();
        unsigned int endPointY = endPointPixel.get<1>();

        QPainterPath painterPath;
        painterPath.moveTo(lastX, lastY);


        if (graphicItems == &_routePaths && path == paths.back()) {
            // last path => paint arrow!
            // arrow head
            if (path->bezierControlPoint().get<0>() != DUMMY_POINT.get<0>()) {
                lastX = path->bezierControlPoint().get<0>();
                lastY = path->bezierControlPoint().get<1>();
            }
            double angle = std::atan2(endPointY-lastY, -(endPointX-lastX));

            double routeWidth = PBCConfig::getInstance()->routeWidth();
            double arrowSize = routeWidth*2;
            QPointF arrowP1 = QPointF(endPointX, endPointY) + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                              cos(angle + M_PI / 3) * arrowSize);
            QPointF arrowP2 = QPointF(endPointX, endPointY) + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                              cos(angle + M_PI - M_PI / 3) * arrowSize);

            QPointF arrowPHalf = arrowP1 + (arrowP2-arrowP1)/2;

            // PBCPathViewArrow Head - Polygon Variante
            QPointF endPoint(endPointX, endPointY);
            QVector<QPointF> pointVector = {endPoint, arrowP1, arrowP2};
            QPolygonF arrowHead(pointVector);
            boost::shared_ptr<QGraphicsPolygonItem> arrowItem(new QGraphicsPolygonItem(arrowHead));
            QPen arrowPen(brush, routeWidth/4);
            arrowItem->setPen(arrowPen);
            arrowItem->setBrush(brush);
            graphicItems->push_back(arrowItem);

            endPointX = arrowPHalf.x();
            endPointY = arrowPHalf.y();
        }

        if ( endPointX < 0 || endPointX >= PBCConfig::getInstance()->canvasWidth()
             || endPointY < 0 || endPointY >= PBCConfig::getInstance()->canvasHeight()) {
            std::ostringstream errMsg;
            errMsg << "trying to draw a route outside of canvas (x = " << endPointX << "; y = " << endPointY << ")";
            throw PBCRenderingException(errMsg.str());
        }

        if (path->bezierControlPoint().get<0>() == DUMMY_POINT.get<0>()) {
            painterPath.lineTo(endPointX, endPointY);
        } else {
            PBCDPoint controlPointYd(inOutFactor * path->bezierControlPoint().get<0>(),
                                     path->bezierControlPoint().get<1>());
            PBCDPoint controlPointPixel = PBCPositionTranslator::getInstance()->translatePos(controlPointYd, basePoint); //NOLINT
            unsigned int controlPointX = controlPointPixel.get<0>();
            unsigned int controlPointY = controlPointPixel.get<1>();
            painterPath.quadTo(QPointF(controlPointX,controlPointY), QPointF(endPointX, endPointY));
        }



        boost::shared_ptr<QGraphicsPathItem> itemSP(new QGraphicsPathItem(painterPath));
        itemSP->setPen(pen);
        graphicItems->push_back(itemSP);
        lastX = endPointX;
        lastY = endPointY;
    }
}

void PBCPlayerView::paintRoute() {
    PBCRouteSP route = _playerSP->route();

    for(QGraphicsItemSP item : _routePaths) {
        this->removeFromGroup(item.get());
    }
    _routePaths.clear();

    PBCDPoint playerPos = PBCPositionTranslator::getInstance()->translatePos(_playerSP->pos()); //NOLINT
    int inOutFactor = -1;
    if(playerPos.get<0>() < PBCConfig::getInstance()->canvasWidth() / 2) {
        inOutFactor = 1;
    }
    if(_playerSP->motion() != NULL) {
        PBCDPoint correctMotionEndPoint(inOutFactor * _playerSP->motion()->motionEndPoint().get<0>(),  //NOLINT
                                        _playerSP->motion()->motionEndPoint().get<1>());               //NOLINT

        PBCDPoint base = PBCPositionTranslator::getInstance()->translatePos(correctMotionEndPoint, playerPos);  // NOLINT
        joinPaths(route->paths(), &_routePaths, base);
    } else {
        joinPaths(route->paths(), &_routePaths, playerPos);
    }

    for(boost::shared_ptr<QGraphicsItem> item : _routePaths) {
        this->addToGroup(item.get());
    }
}


/**
 * @brief Sets a player's route and paints it.
 * @param route The route to apply
 */
void PBCPlayerView::applyRoute(PBCRouteSP route) {
    PBCRouteSP oldRoute = _playerSP->route();
    _playerSP->setRoute(route);
    try {
        paintRoute();
    } catch(const PBCRenderingException& e) {
        _playerSP->setRoute(NULL);
        std::array<char, 4> sn = _playerSP->role().shortName;
        std::string playerShortName(sn.begin(), sn.end());
        QMessageBox::warning(NULL,
                              "Cannot Draw Route",
                              QString::fromStdString("For player \"" +  _playerSP->role().fullName + "\""
                              + " the route \"" + route->name() + "\""
                              + " ends outside the gridiron's borders. "
                                "Please apply another route or change the position/motion of the player."),  // NOLINT
                              QMessageBox::Ok);
    }
}

/**
 * @brief Sets a player's motion and paints it.
 * @param motion The motion to apply
 */
void PBCPlayerView::applyMotion(PBCMotionSP motion) {
    for(boost::shared_ptr<QGraphicsItem> item : _motionPaths) {
        this->removeFromGroup(item.get());
    }
    _motionPaths.clear();

    _playerSP->setMotion(motion);

    PBCDPoint playerPos = PBCPositionTranslator::getInstance()->translatePos(_playerSP->pos()); //NOLINT
    joinPaths(motion->paths(), &_motionPaths, playerPos);

    for(boost::shared_ptr<QGraphicsItem> item : _motionPaths) {
        this->addToGroup(item.get());
    }

    if(_playerSP->route() != NULL) {
        applyRoute(_playerSP->route());
    }
}

/**
 * @brief Changes the color of the player
 * @param color The new color
 */
void PBCPlayerView::setColor(PBCColor color) {
    _playerSP->setColor(color);
    repaint();
}


/**
 * @brief Changes the position of the player
 * @param x The horizontal position in yards
 * @param y The vertical position in yards
 */
void PBCPlayerView::setPosition(double x, double y) {
    PBCDPoint pos(x, y);
    _playerSP->setPos(pos);
    repaint();
}


bool PBCPlayerView::isClickInShape(const QPointF& clickPos) {
    if (_playerShapeSP->contains(clickPos)) {
        std::cout << "contained" << std::endl;
        return true;
    } else {
        std::cout << "not contained" << std::endl;
        return false;
    }
}


/**
 * @brief Handles context menu clicks
 *
 * This is a Qt event hander that is triggered when a right click is performed
 * on an instance of PBCPlayerView. It displays the context menu and detects
 * which menu item has been selected.
 * @param event event datastructure, includes position of context menu click
 */
void PBCPlayerView::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    if (!isClickInShape(event->pos()))  {
        event->ignore();
        return;
    }
    QMenu menu;
    QMenu* routeMenu = menu.addMenu(QString::fromStdString("Routes"));
    boost::unordered_map<QAction*, PBCRouteSP> actionMap;
    std::multimap<int, PBCRouteSP> sortedRoutes;
    for(PBCRouteSP route : PBCController::getInstance()->getPlaybook()->routes()) {
        double depth = route->paths().back()->endpoint().get<1>();
        sortedRoutes.insert(std::make_pair(depth, route));
    }

    for (const auto& kv : sortedRoutes) {
        PBCRouteSP route = kv.second;
        QString routeString = QString::fromStdString(route->name());
        if(route->codeName() != "") {
            routeString.append(" (");
            routeString.append(QString::fromStdString(route->codeName()));
            routeString.append(")");
        }
        QAction* action = routeMenu->addAction(routeString);
        actionMap.insert(std::make_pair(action, route));
    }

    routeMenu->addSeparator();
    QAction* action_CustomRouteCreate_named =
            routeMenu->addAction("Create (named) route");
    QAction* action_CustomRouteCreate_unnamed =
            routeMenu->addAction("Create route (quick, unnamed)");

    QMenu* motionMenu = menu.addMenu(QString::fromStdString("Motions"));
    QAction* action_ApplyMotion = motionMenu->addAction("Apply Motion");
    QAction* action_DeleteMotion = motionMenu->addAction("Delete Motion");


    QAction* action_SetColor = menu.addAction("Set Color");
    QAction* action_SetPosition = menu.addAction("Set Position");
    QAction* clicked = menu.exec(event->screenPos());
    setEnabled(false);
    setEnabled(true);

    bool routeClicked = false;
    for(const auto& kv : actionMap) {
        if(clicked == kv.first) {
            routeClicked = true;
            this->applyRoute(kv.second);
        }
    }
    if(routeClicked == false) {
        if(clicked == action_CustomRouteCreate_unnamed) {
            /*PBCCustomRouteDialog dialog;
            dialog.setWindowModality(Qt::ApplicationModal);
            PBCRouteSP createdRoute = dialog.execute();
            if(createdRoute != NULL) {
                this->applyRoute(createdRoute);
            }*/
            _playView->enterRouteEditMode(this->_playerSP);
        } else if(clicked == action_CustomRouteCreate_named) {
            /*PBCCreateMotionRouteDialog dialog;
            PBCRouteSP createdRoute = dialog.getCreatedRoute();
            if(createdRoute != NULL) {
                this->applyRoute(createdRoute);
            }*/


            PBCSavePlayAsDialog dialog;
            int returnCode = dialog.exec();
            if (returnCode == QDialog::Accepted) {
                PBCSavePlayAsDialog::ReturnStruct rs = dialog.getReturnStruct();
                const auto &routes = PBCController::getInstance()->getPlaybook()->routes();
                bool routeAlreadyInPlaybook = false;
                for (const auto& route : routes) {
                    if (route->name() == rs.name) {
                        routeAlreadyInPlaybook = true;
                        break;
                    }
                }
                bool overwriteRoute = false;
                if (routeAlreadyInPlaybook) {
                    QMessageBox::StandardButton button =
                            QMessageBox::question(NULL,
                                                  "Create custom route",
                                                  QString::fromStdString("There already exists a route named '" + rs.name + "'. Do you want to overwrite it?"),  // NOLINT
                                                  QMessageBox::Ok | QMessageBox::Cancel);

                    if(button == QMessageBox::Ok) {
                        overwriteRoute = true;
                    } else {
                        return;
                    }
                }
                _playView->enterRouteEditMode(this->_playerSP, rs.name, rs.codeName, overwriteRoute);
            }
        } else if(clicked == action_ApplyMotion) {
            /*PBCCreateMotionRouteDialog dialog;
            PBCMotionSP createdMotion = dialog.getCreatedMotion();
            if(createdMotion != NULL) {
                this->applyMotion(createdMotion);
            }*/
            _playView->enterMotionEditMode(this->_playerSP);
        } else if(clicked == action_DeleteMotion) {
            std::vector<PBCPathSP> emptyRoutePaths;
            PBCRouteSP emptyRoute = PBCRouteSP(new PBCRoute("empty", "", emptyRoutePaths));
            this->_playerSP->setRoute(emptyRoute);
            PBCMotionSP emptyMotion(new PBCMotion());
            this->_playerSP->setMotion(emptyMotion);
            this->repaint();
        } else if(clicked == action_ApplyMotion) {} else if(clicked == action_SetColor) {
            QColor color = QColorDialog::getColor(Qt::black);
            if(color.isValid()) {
                this->setColor(PBCColor(color.red(),
                                        color.green(),
                                        color.blue()));
            }
        } else if(clicked == action_SetPosition) {
            bool ok1, ok2;
            double x = QInputDialog::getDouble(NULL,
                                               "Set player position",
                                               "Horizontal position in yd",
                                               0,
                                               -2147483647,
                                               2147483647,
                                               1,
                                               &ok1);
            double y = QInputDialog::getDouble(NULL,
                                               "Set player position",
                                               "Vertical position in yd",
                                               0,
                                               -2147483647,
                                               2147483647,
                                               1,
                                               &ok2);
            if(ok1 == true && ok2 == true) {
                this->setPosition(x, y);
            }
        } else {
            pbcAssert(clicked == NULL);
        }
    }
}




void PBCPlayerView::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (isClickInShape(event->pos())) {
        QGraphicsItemGroup::mousePressEvent(event);
    } else {
        event->ignore();
    }
}

/**
 * @brief Handles Qt events that occur when
 * players' graphical representations of bodies are dragged and dropped.
 * @param event event datastructure, includes position of context menu click
 */
void PBCPlayerView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItemGroup::mouseReleaseEvent(event);
    QPointF pixelDelta = this->pos();
    QPointF newPixelPos = QPointF(_originalPos.get<0>(),
                                  _originalPos.get<1>())
                          + pixelDelta;

    PBCDPoint newPos = PBCPositionTranslator::getInstance()->retranslatePos(PBCDPoint(newPixelPos.x(), newPixelPos.y()));  //NOLINT
    std::cout << pixelDelta.x() << ", " << pixelDelta.y() << std::endl;
    std::cout << newPos.get<0>() << ", " << newPos.get<1>() << std::endl;
    std::cout << "----------------------------------" << std::endl;
    _playerSP->setPos(newPos);
    _playView->setActivePlayer(this->_playerSP);
}


void PBCPlayerView::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    _playView->enterRouteEditMode(this->_playerSP);
}
