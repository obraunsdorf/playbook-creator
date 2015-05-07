#include "pbcCreateMotionRouteDialog.h"
#include "ui_pbcCreateMotionRouteDialog.h"
#include <string>

void PBCCreateMotionRouteDialog::exec() {
    int returnCode = QDialog::exec();
    if(returnCode == QDialog::Rejected) {
        _createdPaths.clear();
    }
}

PBCCreateMotionRouteDialog::PBCCreateMotionRouteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PBCCreateMotionRouteDialog) {
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    exec();
}

PBCCreateMotionRouteDialog::~PBCCreateMotionRouteDialog() {
    delete ui;
}

PBCMotionSP PBCCreateMotionRouteDialog::getCreatedMotion() {
    if(_createdPaths.size() == 0) {
        return NULL;
    } else {
        PBCMotionSP createdMotion(new PBCMotion());
        for(PBCPathSP path : _createdPaths) {
            createdMotion->addPath(path);
        }
        return createdMotion;
    }
}

PBCRouteSP PBCCreateMotionRouteDialog::getCreatedRoute() {
    if(_createdPaths.size() == 0) {
        return NULL;
    } else {
        std::string routeName = ui->nameEdit->text().toStdString();
        std::string routeCodeName = ui->codeNameEdit->text().toStdString();
        return PBCRouteSP(new PBCRoute(routeName,
                                       routeCodeName,
                                       _createdPaths));
    }
}


void PBCCreateMotionRouteDialog::addPath() {
    PBCPathSP path(new PBCPath(ui->xSpinBox->value(),
                               ui->ySpinBox->value(),
                               ui->isArcCheckBox->isChecked()));
    _createdPaths.push_back(path);
    QString s;
    if(path->isArc() == true) {
        s = "arc : ";
    } else {
        s = "line: ";
    }
    s.append(QString::number(path->endpoint().get<0>()));
    s.append(", ");
    s.append(QString::number(path->endpoint().get<1>()));
    ui->listWidget->addItem(s);
}
