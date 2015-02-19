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
    void resize();
    void addPlayerView(PBCPlayerViewSP playerViewSP);
    void showPlay(PBCPlaySP playSP);

private:
    PBCPlaySP _currentPlay;
    std::vector<PBCPlayerViewSP> _playerViewsSP;

};

#endif // PBCGRIDIRONVIEW_H
