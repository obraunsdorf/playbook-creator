#include "pbcGridIronView.h"

#include "util/pbcConfig.h"
#include "gui/pbcPlayerView.h"
#include "QGraphicsEllipseItem"

PBCGridIronView::PBCGridIronView(QObject *parent) :
    QGraphicsScene(parent)
{
    paintLine(PBCConfig::getInstance()->losY(), PBCConfig::getInstance()->losWidth(), PBCConfig::getInstance()->losColor());
    paintLine(PBCConfig::getInstance()->fiveYdY(), PBCConfig::getInstance()->fiveYdWidth(), PBCConfig::getInstance()->fiveYdColor());
    paintBall();
    paintBorder();
}

void PBCGridIronView::paintLine(unsigned int yPos, unsigned int lineWidth, PBCColor color) {
    std::vector<unsigned int> colorVector = color.getColorVector();
    QPen pen(QColor(colorVector[0], colorVector[1], colorVector[2]));
    pen.setWidth(lineWidth);
    this->addLine(0, yPos, PBCConfig::getInstance()->canvasWidth() - pen.width(), yPos, pen);
}

void PBCGridIronView::paintBall()
{
    unsigned int ballX = PBCConfig::getInstance()->canvasWidth() / 2;
    std::vector<unsigned int> ballColorComponents = PBCConfig::getInstance()->ballColor().getColorVector();
    QColor ballColor(ballColorComponents[0], ballColorComponents[1], ballColorComponents[2]);
    QPen ballPen(ballColor);
    QBrush ballBrush(ballColor);
    unsigned int ballWidth = PBCConfig::getInstance()->ballWidth();
    QGraphicsEllipseItem* ball = this->addEllipse(ballX - ballWidth / 2, PBCConfig::getInstance()->losY() - ballWidth, ballWidth, 2 * ballWidth, ballPen, ballBrush);
    ball->setZValue(100);
}

void PBCGridIronView::paintBorder()
{
    this->addRect(0, 0, PBCConfig::getInstance()->canvasWidth(), PBCConfig::getInstance()->canvasHeight());

}

void PBCGridIronView::resize()
{
    this->clear();
    paintLine(PBCConfig::getInstance()->losY(), PBCConfig::getInstance()->losWidth(), PBCConfig::getInstance()->losColor());
    paintLine(PBCConfig::getInstance()->fiveYdY(), PBCConfig::getInstance()->fiveYdWidth(), PBCConfig::getInstance()->fiveYdColor());
    paintBall();
    paintBorder();
    if(_currentPlay != NULL) {
        showPlay();
    }
}


void PBCGridIronView::showPlay()
{
    for(PBCPlayerSP playerSP : *(_currentPlay->formation())) {
        this->addItem(new PBCPlayerView(playerSP));
    }
}

void PBCGridIronView::setCurrentPlay(PBCPlaySP playSP)
{
    _currentPlay = playSP;
    showPlay();
}

