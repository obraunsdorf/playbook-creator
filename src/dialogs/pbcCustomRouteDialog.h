#ifndef PBCCUSTOMROUTEDIALOG_H
#define PBCCUSTOMROUTEDIALOG_H

#include <QDialog>
#include "gui/pbcCustomRouteView.h"
#include "models/pbcRoute.h"

namespace Ui {
class PBCCustomRouteDialog;
}

class PBCCustomRouteDialog : public QDialog {
    Q_OBJECT
 private:
    Ui::PBCCustomRouteDialog *ui;
    boost::shared_ptr<PBCCustomRouteView> _crv;
    PBCRouteSP _createdRoute;

    void savePlaybookOnRouteCreation();

 public:
    explicit PBCCustomRouteDialog(QWidget *parent = 0);
    ~PBCCustomRouteDialog();
    PBCRouteSP exec();

 public slots:
    void accept();
};

#endif  // PBCCUSTOMROUTEDIALOG_H
