/** @file pbcOpenPlayDialog.cpp
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

#include "pbcOpenPlayDialog.h"
#include "ui_pbcOpenPlayDialog.h"
#include "pbcController.h"
#include "pbcController.h"
#include "models/pbcPlaybook.h"
#include "models/pbcPlay.h"
#include "util/pbcDeclarations.h"

#include <list>
#include <map>
#include <string>
#include <QListWidgetItem>
#include <QMessageBox>
#include <boost/regex.hpp>

PBCOpenPlayDialog::PBCOpenPlayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PBCOpenPlayDialog) {
    ui->setupUi(this);
    ui->nameComboBox->setFocus();
    reset();
}

PBCOpenPlayDialog::~PBCOpenPlayDialog() {
    delete ui;
}

PBCOpenPlayDialog::ReturnStruct PBCOpenPlayDialog::getReturnStruct() {
    struct ReturnStruct rs;
    rs.playName = ui->nameComboBox->currentText().toStdString();
    return rs;
}

void PBCOpenPlayDialog::fillCategoryList(std::map<PBCCategorySP, unsigned int> categories,  // NOLINT
                                         unsigned int totalPlayCount) {
    for (const auto& kv : categories) {
        PBCCategorySP categorySP = kv.first;
        unsigned int count = kv.second;
        std::string categoryString = categorySP->name() +
                " (" +
                std::to_string(count) +
                " plays)";
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(categoryString));  // NOLINT
        item->setWhatsThis(QString::fromStdString(categorySP->name()));
        pbcAssert(count <= totalPlayCount)
        if(count == totalPlayCount) {
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        }
        ui->categoryListWidget->addItem(item);  // NOLINT
    }
}

void PBCOpenPlayDialog::filterCategory(QListWidgetItem *item) {
    std::string itemName = item->whatsThis().toStdString();
    ui->nameComboBox->clear();
    ui->codeNameComboBox->clear();
    ui->categoryListWidget->clear();

    PBCCategorySP selectedCategory = PBCController::getInstance()->getPlaybook()->getCategory(itemName);  // NOLINT
    const auto& result = _filteredCategories.insert(selectedCategory);
    pbcAssert(result.second == true);
    std::map<PBCCategorySP, unsigned int> newCategories;
    std::list<PBCPlaySP> newPlays;

    for(PBCPlaySP playSP : _currentPlays) {
        if(playSP->categories().count(selectedCategory) != 0) {
            newPlays.push_back(playSP);
            ui->nameComboBox->addItem(QString::fromStdString(playSP->name()));
            ui->codeNameComboBox->addItem(QString::fromStdString(playSP->codeName()));  // NOLINT
            for(PBCCategorySP playCategorySP : playSP->categories()) {
                if(_filteredCategories.count(playCategorySP) == 0) {
                    newCategories[playCategorySP]++;
                }
            }
        }
    }

    fillCategoryList(newCategories, newPlays.size());
    ui->categoryListWidget->sortItems();
    QString filterLabel = ui->filterLabel->text();
    filterLabel.append(QString::fromStdString("->"+itemName));
    ui->filterLabel->setText(filterLabel);

    _currentPlays = newPlays;
}

void PBCOpenPlayDialog::reset() {
    ui->nameComboBox->clear();
    ui->codeNameComboBox->clear();
    ui->categoryListWidget->clear();

    _currentPlays = PBCController::getInstance()->getPlaybook()->plays();
    if (_currentPlays.empty()) {
        ui->okButton->setEnabled(false);
    }
    _filteredCategories.clear();

    std::map<PBCCategorySP, unsigned int> newCategories;
    for (PBCPlaySP playSP : _currentPlays) {
        ui->nameComboBox->addItem(QString::fromStdString(playSP->name()));
        ui->codeNameComboBox->addItem(QString::fromStdString(playSP->codeName()));  // NOLINT
        for(PBCCategorySP playCategorySP : playSP->categories()) {
            newCategories[playCategorySP]++;
        }
    }

    fillCategoryList(newCategories, _currentPlays.size());
    ui->categoryListWidget->sortItems();
    ui->filterLabel->setText("");
}
