/** @file pbcCreateMotionRouteDialog.cpp
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

#include "pbcCreateMotionRouteDialog.h"
#include "ui_pbcCreateMotionRouteDialog.h"
#include "util/pbcExceptions.h"
#include "models/pbcPlaybook.h"
#include "util/pbcStorage.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <string>

void PBCCreateMotionRouteDialog::execute() {
    int returnCode = QDialog::exec();
    if(returnCode == QDialog::Rejected) {
        _createdPaths.clear();
    }
}

void PBCCreateMotionRouteDialog::saveRoute(PBCRouteSP routeSP) {
    bool successful = false;
    try {
        successful = PBCPlaybook::getInstance()->addRoute(routeSP);
    } catch(const PBCStorageException& e) {
        QMessageBox::information(this, "", "You have to save the playbook to a file before you can add routes");  //NOLINT
        savePlaybookOnRouteCreation();
        return;
    }
    if(successful == false) {
        QMessageBox::StandardButton button =
                QMessageBox::question(this,
                                      "Create custom route",
                                      QString::fromStdString("There already exists a route named '" + routeSP->name() + "'. Do you want to overwrite it?"),  // NOLINT
                                      QMessageBox::Ok | QMessageBox::Cancel);

        if(button == QMessageBox::Ok) {
            bool result;
            try {
                result = PBCPlaybook::getInstance()->addRoute(routeSP,
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
}

void PBCCreateMotionRouteDialog::savePlaybookOnRouteCreation() {
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

PBCCreateMotionRouteDialog::PBCCreateMotionRouteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PBCCreateMotionRouteDialog) {
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    execute();
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
        PBCRouteSP createdRoute(new PBCRoute(routeName,
                                             routeCodeName,
                                             _createdPaths));
        if(routeName != "") {
            saveRoute(createdRoute);
        }
        return createdRoute;
    }
}


void PBCCreateMotionRouteDialog::addPath() {
    PBCPathSP path(new PBCPath(ui->xSpinBox->value(),
                               ui->ySpinBox->value(),
                               ui->isArcCheckBox->isChecked(),
                               ui->isConcaveCheckBox->isChecked()));
    _createdPaths.push_back(path);
    QString s;
    if(path->isArc() == true) {
        if(path->isConcave()) {
            s = "arc(concave): ";
        } else {
            s = "arc(convex): ";
        }
    } else {
        s = "line: ";
    }
    s.append(QString::number(path->endpoint().get<0>()));
    s.append(", ");
    s.append(QString::number(path->endpoint().get<1>()));
    ui->listWidget->addItem(s);
}
