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

#include "pbcExportPdfDialog.h"
#include "ui_pbcExportPdfDialog.h"
#include "models/pbcPlaybook.h"
#include <string>
#include <vector>
#include <QStandardItemModel>
#include <QCheckBox>

PBCExportPDFDialog::PBCExportPDFDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PBCExportPDFDialog) {
    ui->setupUi(this);
    std::vector<std::string> playNames =
            PBCPlaybook::getInstance()->getPlayNames();

    for(std::string& name : playNames) {
        QListWidgetItem* listItem =
                new QListWidgetItem(QString::fromStdString(name),
                                    ui->allPlaysListWidget);
        ui->allPlaysListWidget->addItem(listItem);
    }
    ui->allPlaysListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  //NOLINT
    ui->selectedPlaysListWidget->setSelectionMode(QAbstractItemView::SingleSelection); //NOLINT
}

PBCExportPDFDialog::~PBCExportPDFDialog() {
    delete ui;
}

boost::shared_ptr<QStringList> PBCExportPDFDialog::exec(boost::shared_ptr<ReturnStruct> returnStruct) {  //NOLINT
    int returnCode = QDialog::exec();
    if(returnCode == QDialog::Accepted) {
        returnStruct->paperWidth = ui->paperWidthSpinBox->value();
        returnStruct->paperHeight = ui->paperHeightSpinBox->value();
        returnStruct->columns = ui->columnsSpinBox->value();
        returnStruct->rows = ui->rowsSpinBox->value();
        returnStruct->marginLeft = ui->marginLeftSpinBox->value();
        returnStruct->marginRight = ui->marginRightSpinBox->value();
        returnStruct->marginTop = ui->marginTopSpinBox->value();
        returnStruct->marginBottom = ui->marginBottomSpinBox->value();
        boost::shared_ptr<QStringList> playNamesSP(new QStringList());
        for(int i = 0; i < ui->selectedPlaysListWidget->count(); ++i) {
            QListWidgetItem* item = ui->selectedPlaysListWidget->item(i);
            assert(item != NULL);
            playNamesSP->append(item->text());
        }
        return playNamesSP;
    } else {
        return NULL;
    }
}

void PBCExportPDFDialog::inButtonPressed() {
    QList<QListWidgetItem*> selectedItems = ui->allPlaysListWidget->selectedItems(); //NOLINT
    for(QListWidgetItem* item : selectedItems) {
        ui->selectedPlaysListWidget->addItem(item->text());
    }
    ui->allPlaysListWidget->clearSelection();
}

void PBCExportPDFDialog::outButtonPressed() {
    QList<QListWidgetItem*> selectedItems = ui->selectedPlaysListWidget->selectedItems(); //NOLINT
    assert(selectedItems.size() <= 1);
    for(QListWidgetItem* item : selectedItems) {
        delete item;
        item = NULL;
    }
    ui->selectedPlaysListWidget->clearSelection();
}

void PBCExportPDFDialog::upButtonPressed() {
    QList<QListWidgetItem*> selectedItems = ui->selectedPlaysListWidget->selectedItems(); //NOLINT
    assert(selectedItems.size() <= 1);
    for(QListWidgetItem* item : selectedItems) {
        int row = ui->selectedPlaysListWidget->row(item);
        assert(row >= 0);
        if(row != 0) {
            QListWidgetItem* taken = ui->selectedPlaysListWidget->takeItem(row);
            assert(taken == item);
            ui->selectedPlaysListWidget->insertItem(row - 1, item);
            ui->selectedPlaysListWidget->clearSelection();
            item->setSelected(true);
        }
    }
}

void PBCExportPDFDialog::downButtonPressed() {
    QList<QListWidgetItem*> selectedItems = ui->selectedPlaysListWidget->selectedItems(); //NOLINT
    assert(selectedItems.size() <= 1);
    for(QListWidgetItem* item : selectedItems) {
        int row = ui->selectedPlaysListWidget->row(item);
        assert(row >= 0);
        if(row != ui->selectedPlaysListWidget->count() - 1) {
            QListWidgetItem* taken = ui->selectedPlaysListWidget->takeItem(row);
            assert(taken == item);
            ui->selectedPlaysListWidget->insertItem(row + 1, item);
            ui->selectedPlaysListWidget->clearSelection();
            item->setSelected(true);
        }
    }
}
