/** @file pbcNewPlayDialog.cpp
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

#include "pbcNewPlayDialog.h"
#include "ui_pbcNewPlayDialog.h"
#include "models/pbcPlaybook.h"
#include <vector>
#include <string>

PBCNewPlayDialog::PBCNewPlayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PBCNewPlayDialog) {
    ui->setupUi(this);
    QStringList formationList;
    std::vector<std::string> formationNames =
            PBCPlaybook::getInstance()->getFormationNames();
    if (formationNames.empty()) {
        PBCPlaybook::getInstance()->reloadDefaultFormations();
        formationNames = PBCPlaybook::getInstance()->getFormationNames();
        pbcAssert(formationNames.size() > 0);
    }
    for(std::string& name : formationNames) {
        formationList.append(QString::fromStdString(name));
    }
    ui->formationComboBox->addItems(formationList);

    ui->okButton->setFocus();
}

PBCNewPlayDialog::~PBCNewPlayDialog() {
    delete ui;
}

PBCNewPlayDialog::ReturnStruct PBCNewPlayDialog::getReturnStruct() {
    struct ReturnStruct rs;
    rs.name = ui->nameEdit->text().toStdString();
    rs.codeName = ui->codeNameEdit->text().toStdString();
    rs.formationName = ui->formationComboBox->currentText().toStdString();

    return rs;
}
