/** @file pbcEditCategoriesDialog.h
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

#include "pbcEditCategoriesDialog.h"
#include "ui_pbcEditCategoriesDialog.h"
#include "pbcController.h"
#include "models/pbcPlaybook.h"

#include "models/pbcPlay.h"
#include <string>
#include <QMessageBox>

PBCEditCategoriesDialog::PBCEditCategoriesDialog(PBCPlaySP playSP, QWidget *parent) :  // NOLINT
    QDialog(parent),
    ui(new Ui::PBCEditCategoriesDialog),
    _playSP(playSP) {
    ui->setupUi(this);

    std::string windowTitle = "Categories of \"" +
            _playSP->name() +
            " (" +
            _playSP->codeName() +
            ")";
    this->setWindowTitle(QString::fromStdString(windowTitle));
}

PBCEditCategoriesDialog::~PBCEditCategoriesDialog() {
    delete ui;
}

void PBCEditCategoriesDialog::editCategories() {
    refreshList();
    int returnCode = QDialog::exec();
    if (returnCode == QDialog::Accepted) {
        updateCategoryAssignment();
    }
}

void PBCEditCategoriesDialog::refreshList() {
    ui->categoryListWidget->clear();
    std::list<PBCCategorySP> categories = PBCController::getInstance()->getPlaybook()->categories();  // NOLINT
    for (PBCCategorySP categorySP : categories) {
        QListWidgetItem* item = new QListWidgetItem(
                    QString::fromStdString(categorySP->name()),
                    ui->categoryListWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        for (PBCCategorySP playCategorySP : _playSP->categories()) {
            if(categorySP == playCategorySP) {
                item->setCheckState(Qt::Checked);
            }
        }
        ui->categoryListWidget->addItem(item);
    }
    ui->categoryListWidget->sortItems();
    ui->newCategoryEdit->clear();
}

void PBCEditCategoriesDialog::updateCategoryAssignment() {
    for(int i = 0; i < ui->categoryListWidget->count(); i++) {
        QListWidgetItem* item = ui->categoryListWidget->item(i);
        std::string categoryName = item->text().toStdString();
        PBCCategorySP category = PBCController::getInstance()->getPlaybook()->getCategory(categoryName);  // NOLINT
        if (item->checkState() == Qt::Checked) {
            _playSP->addCategory(category);
            category->addPlay(_playSP);
        } else {
            pbcAssert(item->checkState() == Qt::Unchecked);
            _playSP->removeCategory(category);
            category->removePlay(_playSP);
        }
    }
}


void PBCEditCategoriesDialog::createCategory() {
    std::string categoryName = ui->newCategoryEdit->text().toStdString();
    PBCCategorySP categorySP(new PBCCategory(categoryName));
    _playSP->addCategory(categorySP);
    categorySP->addPlay(_playSP);
    bool successfull = PBCController::getInstance()->getPlaybook()->addCategory(categorySP, false);  // NOLINT
    if(successfull) {
        updateCategoryAssignment();
        refreshList();
    } else {
        QMessageBox::information(
                    this,
                    "",
                    "The category you entered already exists. Please select it in the list above!");  //NOLINT
    }
}
