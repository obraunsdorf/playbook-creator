/** @file pbcCustomRouteDialog.cpp
    This file is part of Playbook Creator.

    Playbook Creator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Playbook Creator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Playbook Creator.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2015 Oliver Braunsdorf

    @author Oliver Braunsdorf
*/

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
        pbcAssert(files.size() == 1);
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

PBCRouteSP PBCCustomRouteDialog::execute() {
    int returnCode = QDialog::exec();
    if(returnCode == QDialog::Accepted) {
        pbcAssert(_createdRoute != NULL);
        return _createdRoute;
    } else {
        return NULL;
    }
}

void PBCCustomRouteDialog::accept() {
    std::string routeName = ui->nameEdit->text().toStdString();
    std::string routeCodeName = ui->codeNameEdit->text().toStdString();
    _createdRoute = _crv->createRoute(routeName, routeCodeName);
    if(routeName != "") {
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
                                          "Create custom route",
                                          QString::fromStdString("There already exists a route named '" + routeName + "'. Do you want to overwrite it?"),  // NOLINT
                                          QMessageBox::Ok | QMessageBox::Cancel); //NOLINT

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
                pbcAssert(result == true);
            } else {
                return;
            }
        }
    }


    QDialog::accept();
}

