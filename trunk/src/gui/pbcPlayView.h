#ifndef PBCPLAYVIEW_H
#define PBCPLAYVIEW_H

#include "gui/pbcGridIronView.h"

#include "models/pbcPlay.h"

class PBCPlayView : public PBCGridIronView
{
public:
    explicit PBCPlayView(QObject *parent = 0);
    void resize();
    void setCurrentPlay(PBCPlaySP playSP = PBCPlaySP(new PBCPlay()));

private:
    PBCPlaySP _currentPlay;
    void showPlay();

};

#endif // PBCPLAYVIEW_H
