#ifndef PBCCUSTOMROUTEVIEW_H
#define PBCCUSTOMROUTEVIEW_H

#include "gui/pbcGridIronView.h"
#include "models/pbcPath.h"
#include "models/pbcRoute.h"
#include <QGraphicsLineItem>

class PBCCustomRouteView : public PBCGridIronView
{
public:
    explicit PBCCustomRouteView(QObject *parent = 0);
    PBCRouteSP createRoute(const std::string& name, const std::string& codeName);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QGraphicsLineItem* _lastLine;
    QPointF _routeStartPos;
    QPointF _lastPressPoint;
    std::vector<PBCPathSP> _paths;
};

#endif // PBCCUSTOMROUTEVIEW_H
