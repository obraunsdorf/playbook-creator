#include "pbcGridIronView.h"
#include "util/pbcConfig.h"
#include <QGraphicsEllipseItem>

PBCGridIronView::PBCGridIronView(QObject *parent) :
    QGraphicsScene(parent)
{
}

void PBCGridIronView::paintLine(unsigned int yPos, unsigned int width, unsigned int lineWidth, PBCColor color) {
    std::vector<unsigned int> colorVector = color.getColorVector();
    QPen pen(QColor(colorVector[0], colorVector[1], colorVector[2]));
    pen.setWidth(lineWidth);
    this->addLine(0, yPos, width - pen.width(), yPos, pen);
}

void PBCGridIronView::paintBall(unsigned int xPos, unsigned int yPos, unsigned int zValue)
{
    std::vector<unsigned int> ballColorComponents = PBCConfig::getInstance()->ballColor().getColorVector();
    QColor ballColor(ballColorComponents[0], ballColorComponents[1], ballColorComponents[2]);
    QPen ballPen(ballColor);
    QBrush ballBrush(ballColor);
    unsigned int ballWidth = PBCConfig::getInstance()->ballWidth();
    QGraphicsEllipseItem* ball = this->addEllipse(xPos - ballWidth / 2, yPos - ballWidth, ballWidth, 2 * ballWidth, ballPen, ballBrush);
    ball->setZValue(zValue);
}

void PBCGridIronView::paintBorder()
{
    this->addRect(0, 0, PBCConfig::getInstance()->canvasWidth(), PBCConfig::getInstance()->canvasHeight());

}
