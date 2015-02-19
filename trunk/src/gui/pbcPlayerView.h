#ifndef PBCPLAYERVIEW_H
#define PBCPLAYERVIEW_H

#include <QGraphicsItemGroup>
#include <QGraphicsSceneContextMenuEvent>
#include <boost/shared_ptr.hpp>
#include "models/pbcPlayer.h"
#include "util/pbcDeclarations.h"

class PBCPlayerView;
typedef boost::shared_ptr<PBCPlayerView> PBCPlayerViewSP;

class PBCPlayerView : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit PBCPlayerView(PBCPlayerSP playerSP);
signals:

private:
    PBCPlayerSP _playerSP;
    boost::shared_ptr<QGraphicsEllipseItem> _playerEllipseSP;
    std::vector<boost::shared_ptr<QGraphicsLineItem>> _routeLines;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void applyRoute(PBCRouteSP route);

};

#endif // PBCPLAYERVIEW_H
