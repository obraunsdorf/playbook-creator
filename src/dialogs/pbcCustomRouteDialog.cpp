#include "pbcCustomRouteDialog.h"
#include "ui_pbcCustomRouteDialog.h"
#include "util/pbcConfig.h"
#include <QDebug>

PBCCustomRouteDialog::PBCCustomRouteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PBCCustomRouteDialog) {
    ui->setupUi(this);

    _crv.reset(new PBCCustomRouteView(ui->graphicsView));
    ui->graphicsView->setScene(_crv.get());
    ui->graphicsView->setFixedWidth(_crv->sceneRect().width() + 2);
    ui->graphicsView->setFixedHeight(_crv->sceneRect().height() + 2);
    qDebug() << this->size();
}

PBCCustomRouteDialog::~PBCCustomRouteDialog() {
    delete ui;
}

PBCRouteSP PBCCustomRouteDialog::exec() {
    int returnCode = QDialog::exec();
    if(returnCode == QDialog::Accepted) {
        assert(_createdRoute != NULL);
        return _createdRoute;
    } else {
        return NULL;
    }
}

void PBCCustomRouteDialog::accept() {
    _createdRoute = _crv->createRoute(ui->nameEdit->text().toStdString(),
                                      ui->codeNameEdit->text().toStdString());
    QDialog::accept();
}

