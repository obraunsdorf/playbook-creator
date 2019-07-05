/** @file pbcExportPdfDialog.cpp
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

#include "pbcDeleteDialog.h"
#include "ui_pbcDeleteDialog.h"
#include "models/pbcPlaybook.h"


PBCDeleteDialog::PBCDeleteDialog(DELETE_ENUM delete_enum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PBCDeleteDialog),
    _nameList(new QStringList()){
    ui->setupUi(this);

    std::vector<std::string> names;
    switch (delete_enum) {
        case DELETE_ENUM::DELETE_FORMATIONS:
            names = PBCPlaybook::getInstance()->getFormationNames();
            break;
        case DELETE_ENUM::DELETE_CATEGORIES:
            names = PBCPlaybook::getInstance()->getCategoryNames();
            break;
        case DELETE_ENUM::DELETE_ROUTES:
            names = PBCPlaybook::getInstance()->getRouteNames();
            break;
        case DELETE_ENUM::DELETE_PLAYS:
            names = PBCPlaybook::getInstance()->getPlayNames();
            break;
        default:
            pbcAssert(false);
    }


    for(const std::string& name : names) {
        QListWidgetItem* listItem =
                new QListWidgetItem(QString::fromStdString(name),
                                    ui->nameListWidget);
        ui->nameListWidget->addItem(listItem);
    }
    ui->nameListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

/**
 * @brief The Destructor
 */
PBCDeleteDialog::~PBCDeleteDialog() {
    delete ui;
}

int PBCDeleteDialog::exec() {  //NOLINT
    int returnCode = QDialog::exec();
    if(returnCode == QDialog::Accepted) {
        for(const auto* item : ui->nameListWidget->selectedItems()) {
            pbcAssert(item != NULL);
            _nameList->append(item->text());
        }
    }
    return returnCode;
}

const boost::shared_ptr<QStringList> PBCDeleteDialog::get_nameList() const {
    return _nameList;
}
