#include "pbcPlayView.h"

#include "util/pbcConfig.h"
#include "gui/pbcPlayerView.h"
#include "QGraphicsEllipseItem"

PBCPlayView::PBCPlayView(QObject *parent) :
    PBCGridIronView(parent)
{
    paintLine(PBCConfig::getInstance()->losY(), PBCConfig::getInstance()->canvasWidth(), PBCConfig::getInstance()->losWidth(), PBCConfig::getInstance()->losColor());
    paintLine(PBCConfig::getInstance()->fiveYdY(), PBCConfig::getInstance()->canvasWidth(), PBCConfig::getInstance()->fiveYdWidth(), PBCConfig::getInstance()->fiveYdColor());
    paintBall(PBCConfig::getInstance()->canvasWidth() / 2, PBCConfig::getInstance()->losY());
    paintBorder();
}


void PBCPlayView::resize()
{
    this->clear();
    paintLine(PBCConfig::getInstance()->losY(), PBCConfig::getInstance()->canvasWidth(), PBCConfig::getInstance()->losWidth(), PBCConfig::getInstance()->losColor());
    paintLine(PBCConfig::getInstance()->fiveYdY(), PBCConfig::getInstance()->canvasWidth(), PBCConfig::getInstance()->fiveYdWidth(), PBCConfig::getInstance()->fiveYdColor());
    paintBall(PBCConfig::getInstance()->canvasWidth() / 2, PBCConfig::getInstance()->losY());
    paintBorder();
    if(_currentPlay != NULL) {
        showPlay();
    }
}


void PBCPlayView::showPlay()
{
    for(PBCPlayerSP playerSP : *(_currentPlay->formation())) {
        this->addItem(new PBCPlayerView(playerSP));
    }
}

void PBCPlayView::setCurrentPlay(PBCPlaySP playSP)
{
    _currentPlay = playSP;
    showPlay();
}

