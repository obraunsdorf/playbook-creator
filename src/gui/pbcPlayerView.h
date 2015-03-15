#ifndef PBCPLAYERVIEW_H
#define PBCPLAYERVIEW_H

#include <QGraphicsItemGroup>
#include <QGraphicsSceneContextMenuEvent>
#include <boost/shared_ptr.hpp>
#include "models/pbcPlayer.h"
#include "util/pbcDeclarations.h"
#include <vector>

class PBCPlayerView;
typedef boost::shared_ptr<PBCPlayerView> PBCPlayerViewSP;

class PBCPlayerView : public QObject, public QGraphicsItemGroup {
    Q_OBJECT
 public:
    explicit PBCPlayerView(PBCPlayerSP playerSP);

 private:
    PBCPlayerSP _playerSP;
    PBCDPoint _originalPos;
    boost::shared_ptr<QGraphicsEllipseItem> _playerEllipseSP;
    std::vector<boost::shared_ptr<QGraphicsLineItem>> _routeLines;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void applyRoute(PBCRouteSP route);
};

#endif  // PBCPLAYERVIEW_H
