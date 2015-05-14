#ifndef PBCCREATEMOTIONROUTEDIALOG_H
#define PBCCREATEMOTIONROUTEDIALOG_H

#include <QDialog>
#include "models/pbcMotion.h"
#include "models/pbcRoute.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace Ui {
class PBCCreateMotionRouteDialog;
}

class PBCCreateMotionRouteDialog : public QDialog {
    Q_OBJECT

 private:
    Ui::PBCCreateMotionRouteDialog *ui;
    std::vector<PBCPathSP> _createdPaths;
    void exec();
    void saveRoute(PBCRouteSP routeSP);
    void savePlaybookOnRouteCreation();

 public:
    explicit PBCCreateMotionRouteDialog(QWidget *parent = 0);
    ~PBCCreateMotionRouteDialog();
    PBCMotionSP getCreatedMotion();
    PBCRouteSP getCreatedRoute();

 public slots:
    void addPath();
};

#endif   // PBCCREATEMOTIONROUTEDIALOG_H
