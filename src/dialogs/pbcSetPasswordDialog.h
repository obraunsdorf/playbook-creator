#ifndef PBCSETPASSWORDDIALOG_H
#define PBCSETPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class PBCSetPasswordDialog;
}

class PBCSetPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PBCSetPasswordDialog(QWidget *parent = 0);
    ~PBCSetPasswordDialog();

    QString getPassword();
private:
    Ui::PBCSetPasswordDialog *ui;
    void accept();
};

#endif // PBCSETPASSWORDDIALOG_H
