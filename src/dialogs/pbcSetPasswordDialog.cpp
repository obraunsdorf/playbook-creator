#include "pbcSetPasswordDialog.h"
#include "ui_pbcSetPasswordDialog.h"
#include <QMessageBox>

PBCSetPasswordDialog::PBCSetPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PBCSetPasswordDialog)
{
    ui->setupUi(this);
}

PBCSetPasswordDialog::~PBCSetPasswordDialog()
{
    delete ui;
}

QString PBCSetPasswordDialog::getPassword() {
    return ui->passwordEdit->text();
}

void PBCSetPasswordDialog::accept() {
    QString pw = ui->passwordEdit->text();
    QString confirm = ui->confirmEdit->text();

    if (pw == confirm) {
        QDialog::accept();
    } else {
        QMessageBox::warning(this,
                             "Save Playbook",
                             "Passwords do not match",
                             QMessageBox::Ok);
    }
}
