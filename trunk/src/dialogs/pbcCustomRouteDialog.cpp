#include "pbcCustomRouteDialog.h"
#include "ui_pbcCustomRouteDialog.h"
#include "util/pbcConfig.h"
#include "util/pbcExceptions.h"
#include "util/pbcStorage.h"
#include "models/pbcPlaybook.h"
#include <QDebug>
#include <QMessageBox>
#include <string>
#include <QFileDialog>
#include <QInputDialog>

void PBCCustomRouteDialog::savePlaybookOnRouteCreation() {
    std::string stdFile = PBCPlaybook::getInstance()->name() + ".pbc";
    QFileDialog fileDialog(
                this, "Save Playbook",
                QString::fromStdString(stdFile),
                "PBC Files (*.pbc);;All Files (*.*)");

    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    if(fileDialog.exec() == true) {
        QStringList files = fileDialog.selectedFiles();
        assert(files.size() == 1);
        QString fileName = files.first();

        bool ok;
        QString password = QInputDialog::getText(this, "Save Playbook",
                                                 "Enter encryption password",
                                                 QLineEdit::Password, "", &ok);
        if(ok == true) {
            PBCStorage::getInstance()->savePlaybook(password.toStdString(),
                                                    fileName.toStdString());
        }
    }
}

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
    std::string routeName = ui->nameEdit->text().toStdString();
    std::string routeCodeName = ui->codeNameEdit->text().toStdString();
    _createdRoute = _crv->createRoute(routeName, routeCodeName);
    bool successful = false;
    try {
        successful = PBCPlaybook::getInstance()->addRoute(_createdRoute);
    } catch(const PBCStorageException& e) {
        QMessageBox::information(this, "", "You have to save the playbook to a file before you can add routes");  //NOLINT
        savePlaybookOnRouteCreation();
        return;
    }
    if(successful == false) {
        QMessageBox::StandardButton button =
                QMessageBox::question(this,
                                      "blub",
                                      QString::fromStdString("There already exists a route named '" + routeName + "'. Do you want to overwrite it?"),  // NOLINT
                                      QMessageBox::Ok | QMessageBox::Cancel);

        if(button == QMessageBox::Ok) {
            bool result;
            try {
                result = PBCPlaybook::getInstance()->addRoute(_createdRoute,
                                                              true);
            } catch(const PBCStorageException& e) {
                QMessageBox::information(this, "", "You have to save the playbook to a file before you can add routes");  //NOLINT
                savePlaybookOnRouteCreation();
                return;
            }
            assert(result == true);
        } else {
            return;
        }
    }


    QDialog::accept();
}

