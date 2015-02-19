#include "pbcPlayerView.h"
#include "QBrush"
#include "QMenu"
#include "util/pbcPositionTranslator.h"



PBCPlayerView::PBCPlayerView(PBCPlayerSP playerSP) : _playerSP(playerSP)
{
    PBCDPoint pos = PBCPositionTranslator::getInstance()->translatePos(_playerSP->pos());
    _playerEllipseSP.reset(new QGraphicsEllipseItem(pos.get<0>() - 10, pos.get<1>(), 20, 20));
    std::vector<unsigned int> color = _playerSP->color().getColorVector();
    _playerEllipseSP->setBrush(QBrush(QColor(color[0],color[1],color[2])));
    this->addToGroup(_playerEllipseSP.get());
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
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
        unsigned int newX = baseX < PBCConfig::getInstance()->canvasWidth() / 2 ? pos.get<0>() : 2 * baseX - pos.get<0>();
        unsigned int newY = pos.get<1>();
        _routeLines.push_back(boost::shared_ptr<QGraphicsLineItem>(new QGraphicsLineItem(lastX, lastY, newX, newY)));
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

    std::vector<PBCPathSP> path_5In({ PBCPathSP(new PBCPath(0,5)), PBCPathSP(new PBCPath(2,5))});
    PBCRouteSP route_5In(new PBCRoute("5 In", "", path_5In));
    QAction* action_5In = routeMenu->addAction(QString::fromStdString(route_5In->name()));

    std::vector<PBCPathSP> path_Post({ PBCPathSP(new PBCPath(0,5)), PBCPathSP(new PBCPath(2,7))});
    PBCRouteSP route_Post(new PBCRoute("Post", "", path_Post));
    QAction* action_Post = routeMenu->addAction(QString::fromStdString(route_Post->name()));

    std::vector<PBCPathSP> path_Slant({ PBCPathSP(new PBCPath(0,2)), PBCPathSP(new PBCPath(2,4))});
    PBCRouteSP route_Slant(new PBCRoute("Slant", "", path_Slant));
    QAction* action_Slant = routeMenu->addAction(QString::fromStdString(route_Slant->name()));

    std::vector<PBCPathSP> path_Fly({ PBCPathSP(new PBCPath(0,7))});
    PBCRouteSP route_Fly(new PBCRoute("Fly", "", path_Fly));
    QAction* action_Fly = routeMenu->addAction(QString::fromStdString(route_Fly->name()));

    // QAction* motion = menu.addAction(QString("Apply Motion"));
    QAction* clicked = menu.exec(event->screenPos());
    setEnabled(false);
    setEnabled(true);

    if(clicked == action_5In) {
        this->applyRoute(route_5In);
    } else if(clicked == action_Post) {
        this->applyRoute(route_Post);
    } else if(clicked == action_Slant) {
        this->applyRoute(route_Slant);
    } else if(clicked == action_Fly) {
        this->applyRoute(route_Fly);
    } else {
        assert(clicked == NULL);
    }
}
