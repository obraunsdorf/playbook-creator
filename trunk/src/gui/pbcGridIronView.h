#ifndef PBCGRIDIRONVIEW_H
#define PBCGRIDIRONVIEW_H

#include <QGraphicsScene>

#include <boost/shared_ptr.hpp>

class PBCGridIronView;
typedef boost::shared_ptr<PBCGridIronView> PBCGridIronViewSP;

class PBCGridIronView : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PBCGridIronView(QObject *parent = 0);
    void resize();

};

#endif // PBCGRIDIRONVIEW_H
