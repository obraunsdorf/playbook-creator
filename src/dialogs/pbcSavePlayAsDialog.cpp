/** @file pbcSavePlayAsDialog.cpp
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

    Copyright 2016 Oliver Braunsdorf

    @author Oliver Braunsdorf
*/

#include "pbcSavePlayAsDialog.h"
#include "ui_pbcSavePlayAsDialog.h"

#include <QMessageBox>

PBCSavePlayAsDialog::PBCSavePlayAsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PBCSavePlayAsDialog) {
    ui->setupUi(this);
    ui->okButton->setFocus();
}

PBCSavePlayAsDialog::~PBCSavePlayAsDialog() {
    delete ui;
}

PBCSavePlayAsDialog::ReturnStruct PBCSavePlayAsDialog::getReturnStruct() {
    struct ReturnStruct rs;
    rs.name = ui->nameEdit->text().toStdString();
    rs.codeName = ui->codeNameEdit->text().toStdString();
    return rs;
}

void PBCSavePlayAsDialog::accept() {
    if (ui->nameEdit->text().isEmpty()) {
        QMessageBox::warning(this,
                             "Save Play As",
                             "Name cannot be empty!",
                             QMessageBox::Ok);
    } else {
        QDialog::accept();
    }
}
