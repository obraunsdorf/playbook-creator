//
// Created by oliver on 12/21/18.
//

#ifndef PLAYBOOKCREATOR_PBCPATHVIEW_H
#define PLAYBOOKCREATOR_PBCPATHVIEW_H


#include <QGraphicsPathItem>

class PBCPathView : public QGraphicsPathItem
{
public:
    PBCPathView(QPointF start, QPointF end, double width, bool joinAtStart = false);

protected:
    QPointF _end;
    QPointF _start;
    double _width;

    QPointF _normalizedVector;
    QPointF _perpendicularVector;
};


#endif //PLAYBOOKCREATOR_PBCPATHVIEW_H
