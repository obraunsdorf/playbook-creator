#ifndef PBCGRIDIRONVIEW_H
#define PBCGRIDIRONVIEW_H

#include <QGraphicsScene>

#include "util/pbcDeclarations.h"
#include "models/pbcColor.h"

class PBCGridIronView : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PBCGridIronView(QObject *parent = 0);

protected:
    void paintLine(unsigned int yPos, unsigned int width, unsigned int lineWidth, PBCColor color);
    void paintBall(unsigned int xPos, unsigned int yPos, unsigned int zValue = 100);
    void paintBorder();
    // void setBackgroundColor()

};

#endif // PBCGRIDIRONVIEW_H
