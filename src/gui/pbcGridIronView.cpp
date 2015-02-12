#include "pbcGridIronView.h"

#include "util/pbcConfig.h"

PBCGridIronView::PBCGridIronView(QObject *parent) :
    QGraphicsScene(parent)
{
    resize();
}

void PBCGridIronView::resize()
{
    this->clear();

    // Line of Scrimmage
    std::vector<unsigned int> losColor = PBCConfig::getInstance()->losColor().getColor();
    QPen losPen(QColor(losColor[0], losColor[1], losColor[2]));
    losPen.setWidth(PBCConfig::getInstance()->losWidth());
    unsigned int losY = PBCConfig::getInstance()->losY();
    this->addLine(0, losY, PBCConfig::getInstance()->canvasWidth() - losPen.width(), losY, losPen);

    // ball
    unsigned int ballX = PBCConfig::getInstance()->canvasWidth() / 2;
    std::vector<unsigned int> ballColorComponents = PBCConfig::getInstance()->ballColor().getColor();
    QColor ballColor(ballColorComponents[0], ballColorComponents[1], ballColorComponents[2]);
    QPen ballPen(ballColor);
    QBrush ballBrush(ballColor);
    unsigned int ballWidth = PBCConfig::getInstance()->ballWidth();
    this->addEllipse(ballX - ballWidth, losY - ballWidth, ballWidth, 2 * ballWidth, ballPen, ballBrush);

    // 5yd line
    std::vector<unsigned int> fiveYdColor = PBCConfig::getInstance()->fiveYdColor().getColor();
    QPen fiveYdPen(QColor(fiveYdColor[0], fiveYdColor[1], fiveYdColor[2]));
    fiveYdPen.setWidth(PBCConfig::getInstance()->fiveYdWidth());
    unsigned int fiveYdY = PBCConfig::getInstance()->fiveYdY();
    this->addLine(0, fiveYdY, PBCConfig::getInstance()->canvasWidth() - fiveYdPen.width(), fiveYdY, fiveYdPen);

    // Border rect
    this->addRect(0, 0, PBCConfig::getInstance()->canvasWidth(), PBCConfig::getInstance()->canvasHeight());
}
