#ifndef PBCGRIDIRONVIEW_H
#define PBCGRIDIRONVIEW_H

#include <QGraphicsScene>

#include <boost/shared_ptr.hpp>
#include <vector>
#include "gui/pbcPlayerView.h"
#include "models/pbcPlay.h"

class PBCGridIronView;
typedef boost::shared_ptr<PBCGridIronView> PBCGridIronViewSP;

class PBCGridIronView : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PBCGridIronView(QObject *parent = 0);
    void paintLine(unsigned int yPos, unsigned int lineWidth, PBCColor color);
    void paintBall();
    void paintBorder();
    void resize();
    void setCurrentPlay(PBCPlaySP playSP = PBCPlaySP(new PBCPlay()));

private:
    PBCPlaySP _currentPlay;
    void showPlay();

};

#endif // PBCGRIDIRONVIEW_H
