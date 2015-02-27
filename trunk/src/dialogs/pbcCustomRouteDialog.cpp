#include "pbcCustomRouteDialog.h"
#include "ui_pbcCustomRouteDialog.h"
#include "util/pbcConfig.h"

PBCCustomRouteDialog::PBCCustomRouteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PBCCustomRouteDialog),
    _crv(new PBCCustomRouteView())
{
    ui->setupUi(this);
    ui->graphicsView->setScene(_crv.get());
}

PBCCustomRouteDialog::~PBCCustomRouteDialog()
{
    delete ui;
}

PBCRouteSP PBCCustomRouteDialog::exec()
{
    int returnCode = QDialog::exec();
    if(returnCode == QDialog::Accepted) {
        assert(_createdRoute != NULL);
        return _createdRoute;
    } else {
        return NULL;
    }
}

void PBCCustomRouteDialog::accept()
{
    _createdRoute = _crv->createRoute(ui->nameEdit->text().toStdString(), ui->codeNameEdit->text().toStdString());
    QDialog::accept();
}

