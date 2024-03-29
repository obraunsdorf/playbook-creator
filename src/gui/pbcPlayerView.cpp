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

    unsigned int playerNr = _playerSP->nr();
    // 0 is the discriminator. Valid player Numbers start from 1.
    // Maybe circumvent this "dirty hack" by using C++17 optionals
    if (playerNr > 0) {
        QGraphicsTextItem* text = new QGraphicsTextItem(QString::fromStdString(std::to_string(_playerSP->nr())));
        QFont font = QFont(QString::fromStdString(PBCConfig::getInstance()->playNameFont()));
        font.setPixelSize(playerWidth/2);
        font.setBold(true);
        text->setFont(font);
        PBCColor contrastColor = PBCColor::contrastColor(color);
        text->setDefaultTextColor(QColor(contrastColor.r(), contrastColor.g(), contrastColor.b()));
        QRectF bdRect = text->boundingRect();
        double textWidth = bdRect.width();
        double textHeight = bdRect.height();
        double x = playerPosX + ((playerWidth - textWidth) / 2);
        double y = playerPosY + ((playerWidth - textHeight) / 2);
        text->setPos(x, y);
        this->addToGroup(text);
        text->setZValue(101.0);
    }
    _playerShapeSP->setZValue(100.0);


    this->setFlag(QGraphicsItem::ItemIsMovable);

    if(_playerSP->motion() != NULL) {
        applyMotion(_playerSP->motion());
    }

    paintRoutes();

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
                              PBCDPoint basePoint,
                              RouteType mode) {
    pbcAssert(graphicItems == &_routePaths || graphicItems == &_motionPaths);
    if (paths.empty()) {
        return;
    }
    PBCColor playerColor = _playerSP->color();
    QColor color;
    switch(mode) {
        case RouteType::Route:
        case RouteType::OptionRoute:
            color = QColor(playerColor.r(), playerColor.g(), playerColor.b()); // QColor("black");
            break;
        case RouteType::Alternative1:
            color = QColor("orange");
            break;
        case RouteType::Alternative2:
            color = QColor("fuchsia");
            break;
    }
    QBrush brush(color);
    Qt::PenStyle style;
    if(graphicItems == &_motionPaths) {
        style = Qt::PenStyle::DashLine;
    } else if (mode == RouteType::OptionRoute) {
        style = Qt::PenStyle::DotLine;
    } else {
        style = Qt::PenStyle::SolidLine;
    }
    QPen pen(brush, PBCConfig::getInstance()->routeWidth(), style, Qt::PenCapStyle::RoundCap, Qt::PenJoinStyle::RoundJoin);

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

void PBCPlayerView::__paintRoutes(PBCRouteSP route, RouteType mode) {
    PBCDPoint playerPos = PBCPositionTranslator::getInstance()->translatePos(_playerSP->pos()); //NOLINT
    int inOutFactor = -1;
    if(playerPos.get<0>() < PBCConfig::getInstance()->canvasWidth() / 2) {
        inOutFactor = 1;
    }
    PBCDPoint base = playerPos;
      /* this could fix issue #31 but would mess up route distances:
       * PBCDPoint base = PBCDPoint(playerPos.get<0>(), playerPos.get<1>() + PBCConfig::getInstance()->playerWidth()/2);
       */
    if(_playerSP->motion() != NULL) {
        PBCDPoint correctMotionEndPoint(inOutFactor * _playerSP->motion()->motionEndPoint().get<0>(),  //NOLINT
                                        _playerSP->motion()->motionEndPoint().get<1>());               //NOLINT

        base = PBCPositionTranslator::getInstance()->translatePos(correctMotionEndPoint, playerPos);  // NOLINT
    }
    joinPaths(route->paths(), &_routePaths, base, mode);

    for(boost::shared_ptr<QGraphicsItem> item : _routePaths) {
        this->addToGroup(item.get());
    }
}

void PBCPlayerView::paintRoutes() {
    for(QGraphicsItemSP item : _routePaths) {
        this->removeFromGroup(item.get());
    }
    _routePaths.clear();

    try {
        for (PBCRouteSP route : _playerSP->optionRoutes()) {
            __paintRoutes(route, RouteType::OptionRoute);
        }
        if (_playerSP->alternativeRoute(2) != NULL) {
            __paintRoutes(_playerSP->alternativeRoute(2), RouteType::Alternative2);
        }
        if (_playerSP->alternativeRoute(1) != NULL) {
            __paintRoutes(_playerSP->alternativeRoute(1), RouteType::Alternative1);
        }
        if (_playerSP->route() != NULL) {
            __paintRoutes(_playerSP->route(), RouteType::Route);
        }
    } catch(const PBCRenderingException& e) {
        _playerSP->resetOptionRoutes();
        _playerSP->resetRoute();
        _playerSP->resetAlternativeRoute(1);
        _playerSP->resetAlternativeRoute(2);
        std::array<char, 4> sn = _playerSP->role().shortName;
        std::string playerShortName(sn.begin(), sn.end());
        QMessageBox::warning(NULL,
                             "Cannot Draw Route",
                             QString::fromStdString("For player \"" +  _playerSP->role().fullName + "\""
                                                    + " the route or motion "
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
    joinPaths(motion->paths(), &_motionPaths, playerPos, RouteType::Route);

    for(boost::shared_ptr<QGraphicsItem> item : _motionPaths) {
        this->addToGroup(item.get());
    }

    if(_playerSP->route() != NULL) {
        paintRoutes();
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

PBCRouteActionMap fillRouteMenu(QMenu* menu, const std::multimap<int, PBCRouteSP> &sortedRoutes) {
    QAction* action_ResetRoutes = menu->addAction(ACTION_TEXT_RESET);
    menu->addSeparator();
    PBCRouteActionMap routeActionMap;
    for (const auto& kv : sortedRoutes) {
        PBCRouteSP route = kv.second;
        QString routeString = QString::fromStdString(route->name());
        if(route->codeName() != "") {
            routeString.append(" (");
            routeString.append(QString::fromStdString(route->codeName()));
            routeString.append(")");
        }
        QAction* action = menu->addAction(routeString);
        routeActionMap.insert(std::make_pair(action, route));
    }
    menu->addSeparator();

    QAction* action_CustomRouteCreate_named =
        menu->addAction(ACTION_TEXT_NAMED_ROUTE);
    QAction* action_CustomRouteCreate_unnamed =
        menu->addAction(ACTION_TEXT_UNNAMED_ROUTE);

    return routeActionMap;
}


void PBCPlayerView::createNamedRoute(RouteType mode) {
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
        _playView->enterRouteEditMode(this->_playerSP, mode, rs.name, rs.codeName, overwriteRoute);
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

    std::multimap<int, PBCRouteSP> sortedRoutes;
    for(PBCRouteSP route : PBCController::getInstance()->getPlaybook()->routes()) {
        double depth = route->paths().back()->endpoint().get<1>();
        sortedRoutes.insert(std::make_pair(depth, route));
    }

    QMenu menu;
    QMenu* routeMenu = menu.addMenu(QString::fromStdString("Route"));
    PBCRouteActionMap routeActionMap = fillRouteMenu(routeMenu, sortedRoutes);

    QMenu* optionRoutesMenu = menu.addMenu(QString::fromStdString("Option Route"));
    PBCRouteActionMap optionRoutesActionMap = fillRouteMenu(optionRoutesMenu, sortedRoutes);

    QMenu* alternative1Menu = menu.addMenu(QString::fromStdString("Alternative Route 1"));
    PBCRouteActionMap alternativeRoute1ActionMap = fillRouteMenu(alternative1Menu, sortedRoutes);

    QMenu* alternative2Menu = menu.addMenu(QString::fromStdString("Alternative Route 2"));
    PBCRouteActionMap alternativeRoute2ActionMap = fillRouteMenu(alternative2Menu, sortedRoutes);

    menu.addSeparator();


    QMenu* motionMenu = menu.addMenu(QString::fromStdString("Motions"));
    QAction* action_ApplyMotion = motionMenu->addAction("Apply Motion");
    QAction* action_DeleteMotion = motionMenu->addAction("Delete Motion");

    menu.addSeparator();

    QAction* action_SetColor = menu.addAction("Set Color");
    QAction* action_SetPosition = menu.addAction("Set Position");
    QAction* clicked = menu.exec(event->screenPos());
    setEnabled(false);
    setEnabled(true);

    if (clicked == NULL) {
        return;
    }


    for(const auto& kv : routeActionMap) {
        if(clicked == kv.first) {
            _playerSP->setRoute(kv.second);
            paintRoutes();
            return;
        }
    }
    for(const auto& kv : optionRoutesActionMap) {
        if(clicked == kv.first) {
            _playerSP->addOptionRoute(kv.second);
            paintRoutes();
            return;
        }
    }
    for(const auto& kv : alternativeRoute1ActionMap) {
        if(clicked == kv.first) {
            _playerSP->setAlternativeRoute(1, kv.second);
            paintRoutes();
            return;
        }
    }
    for(const auto& kv : alternativeRoute2ActionMap) {
        if(clicked == kv.first) {
            _playerSP->setAlternativeRoute(2, kv.second);
            paintRoutes();
            return;
        }
    }

    QWidget* parentWidget = clicked->parentWidget();
    if (QMenu* clickedMenu = dynamic_cast<QMenu*>(parentWidget)) {
        if (clickedMenu == routeMenu) {
            if (clicked->text() == ACTION_TEXT_RESET) {
                _playerSP->resetRoute();
                paintRoutes();
            } else if (clicked->text() == ACTION_TEXT_NAMED_ROUTE) {
                createNamedRoute(RouteType::Route);
            } else if (clicked->text() == ACTION_TEXT_UNNAMED_ROUTE) {
                _playView->enterRouteEditMode(this->_playerSP, RouteType::Route);
            }
        } else if (clickedMenu == optionRoutesMenu) {
            std::cout << "choose option menu" << std::endl;
            if (clicked->text() == ACTION_TEXT_RESET) {
                _playerSP->resetOptionRoutes();
                paintRoutes();
            } else if (clicked->text() == ACTION_TEXT_NAMED_ROUTE) {
                createNamedRoute(RouteType::OptionRoute);
            } else if (clicked->text() == ACTION_TEXT_UNNAMED_ROUTE) {
                _playView->enterRouteEditMode(this->_playerSP, RouteType::OptionRoute);
            }
        } else if (clickedMenu == alternative1Menu) {
            if (clicked->text() == ACTION_TEXT_RESET) {
                _playerSP->resetAlternativeRoute(1);
                paintRoutes();
            } else if (clicked->text() == ACTION_TEXT_NAMED_ROUTE) {
                createNamedRoute(RouteType::Alternative1);
            } else if (clicked->text() == ACTION_TEXT_UNNAMED_ROUTE) {
                _playView->enterRouteEditMode(this->_playerSP, RouteType::Alternative1);
            }
        } else if (clickedMenu == alternative2Menu) {
            if (clicked->text() == ACTION_TEXT_RESET) {
                _playerSP->resetAlternativeRoute(2);
                paintRoutes();
            } else if (clicked->text() == ACTION_TEXT_NAMED_ROUTE) {
                createNamedRoute(RouteType::Alternative2);
            } else if (clicked->text() == ACTION_TEXT_UNNAMED_ROUTE) {
                _playView->enterRouteEditMode(this->_playerSP, RouteType::Alternative2);
            }
        }
    }
    
    if(clicked == action_ApplyMotion) {
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
    }
}




void PBCPlayerView::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    std::cout << "mouse press event on " << this->_playerSP->role().fullName << std::endl;
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
    std::cout << "mouse release event on " << this->_playerSP->role().fullName << std::endl;
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
    std::cout << "mouse double click event on " << this->_playerSP->role().fullName << std::endl;
    if (isClickInShape(event->pos())) {
        QGraphicsItemGroup::mouseDoubleClickEvent(event);
        _playView->enterRouteEditMode(this->_playerSP, RouteType::Route);
    } else {
        event->ignore();
    }
}
