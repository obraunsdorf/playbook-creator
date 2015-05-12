#include "pbcPlayerView.h"

#include "models/pbcPlaybook.h"
#include "QBrush"
#include "QMenu"
#include "util/pbcPositionTranslator.h"
#include "dialogs/pbcCustomRouteDialog.h"
#include <boost/unordered/unordered_map.hpp>
#include <utility>
#include "QDebug"
#include <vector>
#include <set>
#include "dialogs/pbcCreateMotionRouteDialog.h"
#include <QColorDialog>


PBCPlayerView::PBCPlayerView(PBCPlayerSP playerSP) : _playerSP(playerSP) {
    repaint();
}

void PBCPlayerView::repaint() {
    _originalPos = PBCPositionTranslator::getInstance()->translatePos(_playerSP->pos());  // NOLINT
    unsigned int playerWidth = PBCConfig::getInstance()->playerWidth();
    double playerPosX = _originalPos.get<0>() - playerWidth / 2;
    double playerPosY = _originalPos.get<1>();
    _playerEllipseSP.reset(new QGraphicsEllipseItem(playerPosX,
                                                    playerPosY,
                                                    playerWidth,
                                                    playerWidth));

    PBCColor color = _playerSP->color();
    _playerEllipseSP->setBrush(QBrush(QColor(color.r(), color.g(), color.b())));
    this->addToGroup(_playerEllipseSP.get());
    this->setFlag(QGraphicsItem::ItemIsMovable);

    if(_playerSP->motion() != NULL) {
        applyMotion(_playerSP->motion());
    }
    if(_playerSP->route() != NULL) {
        applyRoute(_playerSP->route());
    }
}

void PBCPlayerView::joinPaths(const std::vector<PBCPathSP>& paths,
                              std::vector<QGraphicsItemSP>* graphicItems,
                              PBCDPoint basePoint) {
    assert(graphicItems == &_routePaths || graphicItems == &_motionPaths);
    PBCColor color = _playerSP->color();
    QPen pen((QColor(color.r(), color.g(), color.b())));
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
        if(path->isArc() == true) {
            PBCDPoint topleftYd;
            PBCDPoint bottomrightYd;
            if(endPointX > baseX) {
                // motion to the right
                topleftYd =     PBCDPoint(0,
                                          -1 * endPointYd.get<1>());
                bottomrightYd = PBCDPoint(2 * endPointYd.get<0>(),
                                          endPointYd.get<1>());

            } else if(endPointX < baseX) {
                // motion to the left
                topleftYd =     PBCDPoint(2 * endPointYd.get<0>(),
                                          -1 * endPointYd.get<1>());
                bottomrightYd = PBCDPoint(0,
                                          endPointYd.get<1>());

            } else {
                assert(false);  // TODO(obr): message to user: no strait motions
            }
            PBCDPoint topleftPixel = PBCPositionTranslator::getInstance()->translatePos(topleftYd, PBCDPoint(lastX, lastY)); //NOLINT
            PBCDPoint bottomrightPixel = PBCPositionTranslator::getInstance()->translatePos(bottomrightYd, PBCDPoint(lastX, lastY)); //NOLINT
            QPointF topleft(topleftPixel.get<0>(),
                            topleftPixel.get<1>());
            QPointF bottomright(bottomrightPixel.get<0>(),
                                bottomrightPixel.get<1>());
            QRectF arcRect(topleft, bottomright);
            QPainterPath motionArc(QPointF(topleft.x(), topleft.y()));
            motionArc.arcMoveTo(arcRect, 0);
            motionArc.arcTo(arcRect, 0, -90);
            boost::shared_ptr<QGraphicsPathItem> motionPathSP(
                        new QGraphicsPathItem(motionArc, this));

            motionPathSP->setPen(pen);
            graphicItems->push_back(motionPathSP);
            /*graphicItems->push_back(boost::shared_ptr<QGraphicsRectItem>(
                                         new QGraphicsRectItem(arcRect)));*/
            lastX = motionArc.currentPosition().x();
            lastY = motionArc.currentPosition().y();
            /*graphicItems->push_back(boost::shared_ptr<QGraphicsEllipseItem>(
                                        new QGraphicsEllipseItem(lastX,
                                                                 lastY,
                                                                 20, 20)));*/
        } else {
            boost::shared_ptr<QGraphicsLineItem> lineSP(
                        new QGraphicsLineItem(lastX,
                                              lastY,
                                              endPointX,
                                              endPointY));

            lineSP->setPen(pen);
            graphicItems->push_back(lineSP);
            lastX = endPointX;
            lastY = endPointY;
        }
    }
}

void PBCPlayerView::applyRoute(PBCRouteSP route) {
    for(QGraphicsItemSP item : _routePaths) {
        this->removeFromGroup(item.get());
    }
    _routePaths.clear();

    _playerSP->setRoute(route);

    PBCDPoint playerPos = PBCPositionTranslator::getInstance()->translatePos(_playerSP->pos()); //NOLINT
    if(_playerSP->motion() != NULL) {
        PBCDPoint base = PBCPositionTranslator::getInstance()->translatePos(_playerSP->motion()->motionEndPoint(), playerPos);  // NOLINT
        joinPaths(route->paths(), &_routePaths, base);
    } else {
        joinPaths(route->paths(), &_routePaths, playerPos);
    }

    for(boost::shared_ptr<QGraphicsItem> item : _routePaths) {
        this->addToGroup(item.get());
    }
}

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

void PBCPlayerView::setColor(PBCColor color) {
    _playerSP->setColor(color);
    repaint();
}


void PBCPlayerView::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QMenu menu;
    QMenu* routeMenu = menu.addMenu(QString::fromStdString("Apply Route"));
    boost::unordered_map<QAction*, PBCRouteSP> actionMap;
    for(PBCRouteSP route : PBCPlaybook::getInstance()->routes()) {
        QAction* action = routeMenu->addAction(
                    QString::fromStdString(route->name()));
        actionMap.insert(std::make_pair(action, route));
    }

    routeMenu->addSeparator();
    QAction* action_CustomRouteCreate =
            routeMenu->addAction("Create custom route");
    QAction* action_CustomRouteCreate_Graphical =
            routeMenu->addAction("Create custom route (graphical)");

    QAction* action_ApplyMotion = menu.addAction(QString("Apply Motion"));
    QAction* action_SetColor = menu.addAction(QString("Set Color"));
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
        if(clicked == action_CustomRouteCreate_Graphical) {
            PBCCustomRouteDialog dialog;
            dialog.setWindowModality(Qt::ApplicationModal);
            PBCRouteSP createdRoute = dialog.exec();
            if(createdRoute != NULL) {
                this->applyRoute(createdRoute);
            }
        } else if(clicked == action_CustomRouteCreate) {
            PBCCreateMotionRouteDialog dialog;
            PBCRouteSP createdRoute = dialog.getCreatedRoute();
            if(createdRoute != NULL) {
                this->applyRoute(createdRoute);
            }
        } else if(clicked == action_ApplyMotion) {
            PBCCreateMotionRouteDialog dialog;
            PBCMotionSP createdMotion = dialog.getCreatedMotion();
            if(createdMotion != NULL) {
                this->applyMotion(createdMotion);
            }
        } else if(clicked == action_SetColor) {
            QColor color = QColorDialog::getColor(Qt::black);
            if(color.isValid()) {
                this->setColor(PBCColor(color.red(),
                                        color.green(),
                                        color.blue()));
            }
        } else {
            assert(clicked == NULL);
        }
    }
}

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
}
