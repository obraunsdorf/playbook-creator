#include "pbcPlayerView.h"

#include "models/pbcPlaybook.h"
#include "QBrush"
#include "QMenu"
#include "util/pbcPositionTranslator.h"
#include "dialogs/pbcCustomRouteDialog.h"
#include <boost/unordered/unordered_map.hpp>
#include <utility>
#include "QDebug"


PBCPlayerView::PBCPlayerView(PBCPlayerSP playerSP) : _playerSP(playerSP)
{
    _originalPos = PBCPositionTranslator::getInstance()->translatePos(_playerSP->pos());
    unsigned int playerWidth = PBCConfig::getInstance()->playerWidth();
    _playerEllipseSP.reset(new QGraphicsEllipseItem(_originalPos.get<0>() - playerWidth / 2, _originalPos.get<1>(), playerWidth, playerWidth));
    std::vector<unsigned int> color = _playerSP->color().getColorVector();
    _playerEllipseSP->setBrush(QBrush(QColor(color[0],color[1],color[2])));
    this->addToGroup(_playerEllipseSP.get());
    this->setFlag(QGraphicsItem::ItemIsMovable);

    if(_playerSP->route() != NULL) {
        applyRoute(_playerSP->route());
    }
}

void PBCPlayerView::applyRoute(PBCRouteSP route)
{
    for(boost::shared_ptr<QGraphicsLineItem> line : _routeLines) {
        this->removeFromGroup(line.get());
    }
    _routeLines.clear();

    _playerSP->setRoute(route);
    QRectF ellipseRect = _playerEllipseSP->rect();
    double baseX = ellipseRect.x() + _playerEllipseSP->scenePos().x() + ellipseRect.width() / 2;
    double baseY = ellipseRect.y() + _playerEllipseSP->scenePos().y();
    double lastX = baseX;
    double lastY = baseY;
    for(PBCPathSP path : route->paths()) {
        PBCDPoint pos = PBCPositionTranslator::getInstance()->translatePos(path->endpoint(), PBCDPoint(baseX, baseY));
        int newX = baseX < PBCConfig::getInstance()->canvasWidth() / 2 ? pos.get<0>() : 2 * baseX - pos.get<0>();
        int newY = pos.get<1>();
        boost::shared_ptr<QGraphicsLineItem> lineSP(new QGraphicsLineItem(lastX, lastY, newX, newY));
        std::vector<unsigned int> colorVector = _playerSP->color().getColorVector();
        lineSP->setPen(QPen(QColor(colorVector[0], colorVector[1], colorVector[2])));
        _routeLines.push_back(lineSP);
        lastX = newX;
        lastY = newY;
    }

    for(boost::shared_ptr<QGraphicsLineItem> line : _routeLines) {
        this->addToGroup(line.get());
    }

}


void PBCPlayerView::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QMenu menu;
    QMenu* routeMenu = menu.addMenu(QString::fromStdString("Apply Route"));
    boost::unordered_map<QAction*, PBCRouteSP> actionMap;
    for(PBCRouteSP route : PBCPlaybook::getInstance()->routes()) {
        QAction* action = routeMenu->addAction(QString::fromStdString(route->name()));
        actionMap.insert(std::make_pair(action, route));
    }

    routeMenu->addSeparator();
    QAction* action_CustomRouteCreate = routeMenu->addAction("Create Custom Route");
    // QAction* motion = menu.addAction(QString("Apply Motion"));
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
        } else {
            assert(clicked == NULL);
        }
    }
}

void PBCPlayerView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItemGroup::mouseReleaseEvent(event);
    QPointF pixelDelta = this->pos();
    QPointF newPixelPos = QPointF(_originalPos.get<0>(), _originalPos.get<1>()) + pixelDelta;
    PBCDPoint newPos = PBCPositionTranslator::getInstance()->retranslatePos(PBCDPoint(newPixelPos.x(), newPixelPos.y()));
    std::cout << pixelDelta.x() << ", " << pixelDelta.y() << std::endl;
    std::cout << newPos.get<0>() << ", " << newPos.get<1>() << std::endl;
    std::cout << "----------------------------------" << std::endl;
    _playerSP->setPos(newPos);
}

