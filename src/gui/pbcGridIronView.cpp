#include "pbcGridIronView.h"
#include "util/pbcConfig.h"
#include <QGraphicsEllipseItem>
#include <vector>

PBCGridIronView::PBCGridIronView(QObject *parent) :
    QGraphicsScene(parent) {}

void PBCGridIronView::paintLine(unsigned int yPos,
                                unsigned int width,
                                unsigned int lineWidth,
                                PBCColor color) {
    QPen pen(QColor(color.r(), color.g(), color.b()));
    pen.setWidth(lineWidth);
    this->addLine(0, yPos, width - pen.width(), yPos, pen);
}

void PBCGridIronView::paintBall(unsigned int xPos,
                                unsigned int yPos,
                                unsigned int zValue) {
    PBCColor color = PBCConfig::getInstance()->ballColor();
    QColor ballColor(color.r(), color.g(), color.b());
    QPen ballPen(ballColor);
    QBrush ballBrush(ballColor);
    unsigned int ballWidth = PBCConfig::getInstance()->ballWidth();
    QGraphicsEllipseItem* ball = this->addEllipse(xPos - ballWidth / 2,
                                                  yPos - ballWidth, ballWidth,
                                                  2 * ballWidth,
                                                  ballPen,
                                                  ballBrush);
    ball->setZValue(zValue);
}

void PBCGridIronView::paintBorder() {
    this->addRect(0,
                  0,
                  PBCConfig::getInstance()->canvasWidth(),
                  PBCConfig::getInstance()->canvasHeight());
}
