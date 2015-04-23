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
                                    ui->listWidget);
        ui->listWidget->addItem(listItem);
    }
    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
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
        returnStruct->rows = ui->columnsSpinBox->value();
        returnStruct->marginLeft = ui->marginLeftSpinBox->value();
        returnStruct->marginRight = ui->marginRightSpinBox->value();
        returnStruct->marginTop = ui->marginTopSpinBox->value();
        returnStruct->marginBottom = ui->marginBottomSpinBox->value();
        boost::shared_ptr<QStringList> playNamesSP(new QStringList());
        QList<QListWidgetItem*> selectedItems = ui->listWidget->selectedItems();
        for(QListWidgetItem* item : selectedItems) {
                playNamesSP->append(item->text());
        }
        return playNamesSP;
    } else {
        return NULL;
    }
}
