#include "pbcCustomRouteView.h"
#include "util/pbcConfig.h"
#include "util/pbcPositionTranslator.h"
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>

PBCCustomRouteView::PBCCustomRouteView(QObject *parent) :
    QGraphicsScene(parent),
    _lastLine(NULL)
{
    unsigned int height = 15 * PBCConfig::getInstance()->ydInPixel();
    unsigned int width = 10 * PBCConfig::getInstance()->ydInPixel();
    /*QGraphicsView* parentView = dynamic_cast<QGraphicsView*>(parent);
    assert(parentView != NULL);
    parentView->setFixedHeight(height);
    parentView->setFixedWidth(width);*/
    // this->setSceneRect(0, 0, height, width);

    // Line of Scrimmage
    std::vector<unsigned int> losColor = PBCConfig::getInstance()->losColor().getColorVector();
    QPen losPen(QColor(losColor[0], losColor[1], losColor[2]));
    losPen.setWidth(PBCConfig::getInstance()->losWidth());
    unsigned int losY = height - 2 * PBCConfig::getInstance()->ydInPixel();
    this->addLine(0, losY, width - losPen.width() - 10, losY, losPen);

    // ball
    unsigned int ballWidth = PBCConfig::getInstance()->ballWidth();
    unsigned int ballX = width - ballWidth;
    std::vector<unsigned int> ballColorComponents = PBCConfig::getInstance()->ballColor().getColorVector();
    QColor ballColor(ballColorComponents[0], ballColorComponents[1], ballColorComponents[2]);
    QPen ballPen(ballColor);
    QBrush ballBrush(ballColor);
    this->addEllipse(ballX - ballWidth / 2, losY - ballWidth, ballWidth, 2 * ballWidth, ballPen, ballBrush);

    // 5yd line
    std::vector<unsigned int> fiveYdColor = PBCConfig::getInstance()->fiveYdColor().getColorVector();
    QPen fiveYdPen(QColor(fiveYdColor[0], fiveYdColor[1], fiveYdColor[2]));
    fiveYdPen.setWidth(PBCConfig::getInstance()->fiveYdWidth());
    unsigned int fiveYdY = losY - 5 * PBCConfig::getInstance()->ydInPixel();
    this->addLine(0, fiveYdY, width - fiveYdPen.width() - 10, fiveYdY, fiveYdPen);

    // Player
    this->addEllipse(this->width() / 2 - 10, losY, 20, 20);
    _routeStartPos = QPointF(this->width() / 2, losY);
    _lastPressPoint = _routeStartPos;
}

PBCRouteSP PBCCustomRouteView::createRoute(const std::string &name, const std::string &codeName)
{
    PBCRouteSP route(new PBCRoute(name, codeName, _paths));
    PBCRoute::addCustomRoute(route);
    return route;
}


void PBCCustomRouteView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(_lastLine != NULL) {
        this->removeItem(_lastLine);
    }
    unsigned int newX = event->scenePos().x();
    unsigned int newY = event->scenePos().y();
    _lastLine =  this->addLine(_lastPressPoint.x(), _lastPressPoint.y(), newX, newY);
}


void PBCCustomRouteView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    unsigned int newX = event->scenePos().x();
    unsigned int newY = event->scenePos().y();
    PBCDPoint pathPoint =  PBCPositionTranslator::getInstance()->retranslatePos(PBCDPoint(newX, newY), PBCDPoint(_routeStartPos.x(), _routeStartPos.y()));
    std::cout << pathPoint.get<0>() << ", " << pathPoint.get<1>() << std::endl;
    _paths.push_back(PBCPathSP(new PBCPath(pathPoint)));
    this->addLine(_lastPressPoint.x(), _lastPressPoint.y(), newX, newY);
    _lastPressPoint.setX(newX);
    _lastPressPoint.setY(newY);
}
