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


PBCPlayerView::PBCPlayerView(PBCPlayerSP playerSP) : _playerSP(playerSP) {
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

void PBCPlayerView::applyRoute(PBCRouteSP route) {
    for(boost::shared_ptr<QGraphicsLineItem> line : _routePaths) {
        this->removeFromGroup(line.get());
    }
    _routePaths.clear();

    _playerSP->setRoute(route);
    QRectF ellipseRect = _playerEllipseSP->rect();
    double playerPosX = ellipseRect.x() +
            _playerEllipseSP->scenePos().x() +  // TODO(obr): check why we need this // NOLINT
            ellipseRect.width() / 2;

    double playerPosY = ellipseRect.y() + _playerEllipseSP->scenePos().y();

    PBCDPoint base;
    if(_playerSP->motion() != NULL) {
        base = PBCPositionTranslator::getInstance()->translatePos(_playerSP->motion()->motionEndPoint(), PBCDPoint(playerPosX, playerPosY));  // NOLINT
    } else {
        base = PBCDPoint(playerPosX, playerPosY);
    }
    double baseX = base.get<0>();
    double baseY = base.get<1>();
    double lastX = baseX;
    double lastY = baseY;
    for(PBCPathSP path : route->paths()) {
        PBCDPoint pos = PBCPositionTranslator::getInstance()->translatePos(path->endpoint(), PBCDPoint(baseX, baseY));  // NOLINT
        int newX = -1;
        if(baseX < PBCConfig::getInstance()->canvasWidth() / 2) {
            newX = pos.get<0>();
        } else {
            newX = 2 * baseX - pos.get<0>();
        }
        int newY = pos.get<1>();
        boost::shared_ptr<QGraphicsLineItem> lineSP(
                    new QGraphicsLineItem(lastX, lastY, newX, newY));
        PBCColor color = _playerSP->color();
        lineSP->setPen(QPen(QColor(color.r(), color.g(), color.b())));
        _routePaths.push_back(lineSP);
        lastX = newX;
        lastY = newY;
    }

    for(boost::shared_ptr<QGraphicsLineItem> line : _routePaths) {
        this->addToGroup(line.get());
    }
}

void PBCPlayerView::applyMotion(PBCMotionSP motion) {
    for(boost::shared_ptr<QGraphicsItem> item : _motionPaths) {
        this->removeFromGroup(item.get());
    }
    _motionPaths.clear();

    _playerSP->setMotion(motion);

    QRectF ellipseRect = _playerEllipseSP->rect();
    double baseX = ellipseRect.x() +
            _playerEllipseSP->scenePos().x() +  // TODO(obr): check why we need this // NOLINT
            ellipseRect.width() / 2;

    double baseY = ellipseRect.y() +
            _playerEllipseSP->scenePos().y();

    double lastX = baseX;
    double lastY = baseY;
    for(PBCPathSP path : *motion) {
        if(path->isArc()) {
            PBCDPoint arcBottom = PBCPositionTranslator::getInstance()->translatePos(path->endpoint(), PBCDPoint(lastX, lastY)); //NOLINT
            QPointF arcTopleft(lastX - 2 * (lastX - arcBottom.get<0>()), lastY - (arcBottom.get<1>() - lastY)); //NOLINT
            QPainterPath motionArc(QPointF(0, 0));
            motionArc.arcMoveTo(arcTopleft.x(),
                                arcTopleft.y(),
                                lastX - arcTopleft.x(),
                                arcBottom.get<1>() - arcTopleft.y(),
                                0);
            motionArc.arcTo(arcTopleft.x(),
                            arcTopleft.y(),
                            lastX - arcTopleft.x(),
                            arcBottom.get<1>() - arcTopleft.y(),
                            0,
                            -90);
            boost::shared_ptr<QGraphicsPathItem> motionPathSP(new QGraphicsPathItem(motionArc, this)); //NOLINT
            PBCColor color = _playerSP->color();
            motionPathSP->setPen(QPen(QBrush(QColor(color.r(),
                                                    color.g(),
                                                    color.b())),
                                      1,
                                      Qt::DashLine));
            _motionPaths.push_back(motionPathSP);
            lastX = motionArc.currentPosition().x();
            lastY = motionArc.currentPosition().y();
        } else {
            PBCDPoint endPoint = PBCPositionTranslator::getInstance()->translatePos(path->endpoint(), PBCDPoint(baseX, baseY));  // NOLINT
            double newX = endPoint.get<0>();
            double newY = endPoint.get<1>();
            boost::shared_ptr<QGraphicsLineItem> lineSP(
                        new QGraphicsLineItem(lastX, lastY, newX, newY));
            PBCColor color = _playerSP->color();
            lineSP->setPen(QPen(QBrush(QColor(color.r(),
                                              color.g(),
                                              color.b())),
                                1,
                                Qt::DashLine));
            _motionPaths.push_back(lineSP);
            lastX = newX;
            lastY = newY;
        }
    }

    for(boost::shared_ptr<QGraphicsItem> item : _motionPaths) {
        this->addToGroup(item.get());
    }

    if(_playerSP->route() != NULL) {
        applyRoute(_playerSP->route());
    }
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
            routeMenu->addAction("Create Custom Route");
    QAction* action_ApplyMotion = menu.addAction(QString("Apply Motion"));
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
        if(clicked == action_CustomRouteCreate) {
            PBCCustomRouteDialog dialog;
            dialog.setWindowModality(Qt::ApplicationModal);
            PBCRouteSP createdRoute = dialog.exec();
            if(createdRoute != NULL) {
                this->applyRoute(createdRoute);
            }
        } else if(clicked == action_ApplyMotion) {
            PBCMotionSP motionSP(new PBCMotion());
            motionSP->addPath(PBCPathSP(new PBCPath(-4, -5, true)));
            motionSP->addPath(PBCPathSP(new PBCPath(-20, -5)));
            this->applyMotion(motionSP);
        } else {
            assert(clicked == NULL);
        }
    }
}

void PBCPlayerView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItemGroup::mouseReleaseEvent(event);
    QPointF pixelDelta = this->pos();
    QPointF newPixelPos = QPointF(_originalPos.get<0>(),
                                  _originalPos.get<1>()) + pixelDelta;
    PBCDPoint newPos = PBCPositionTranslator::getInstance()->retranslatePos(PBCDPoint(newPixelPos.x(), newPixelPos.y()));  //NOLINT
    std::cout << pixelDelta.x() << ", " << pixelDelta.y() << std::endl;
    std::cout << newPos.get<0>() << ", " << newPos.get<1>() << std::endl;
    std::cout << "----------------------------------" << std::endl;
    _playerSP->setPos(newPos);
}

