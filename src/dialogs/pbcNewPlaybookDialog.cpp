/** @file pbcNewPlaybookDialog.cpp
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

#include "pbcNewPlaybookDialog.h"
#include "ui_pbcNewPlaybookDialog.h"

PBCNewPlaybookDialog::PBCNewPlaybookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PBCNewPlaybookDialog) {
    ui->setupUi(this);
    ui->titleEdit->setFocus();
}

PBCNewPlaybookDialog::~PBCNewPlaybookDialog() {
    delete ui;
}

PBCNewPlaybookDialog::ReturnStruct PBCNewPlaybookDialog::getReturnStruct() {
    struct ReturnStruct rs;
    rs.playbookTitle = ui->titleEdit->text().toStdString();
    rs.playerNumber = ui->playerNumberSpinBox->value();
    return rs;
}
