#ifndef PBCEXPORTPDFDIALOG_H
#define PBCEXPORTPDFDIALOG_H

#include <QDialog>
#include <boost/shared_ptr.hpp>

namespace Ui {
class PBCExportPDFDialog;
}

class PBCExportPDFDialog : public QDialog {
    Q_OBJECT

 public:
    explicit PBCExportPDFDialog(QWidget *parent = 0);
    ~PBCExportPDFDialog();

    struct ReturnStruct {
        unsigned int paperWidth;
        unsigned int paperHeight;
        unsigned int columns;
        unsigned int rows;
        unsigned int marginLeft;
        unsigned int marginRight;
        unsigned int marginTop;
        unsigned int marginBottom;
    };

    boost::shared_ptr<QStringList> exec(boost::shared_ptr<ReturnStruct> returnStruct); //NOLINT

 private:
    Ui::PBCExportPDFDialog *ui;

 private slots:
    void inButtonPressed();
    void outButtonPressed();
    void upButtonPressed();
    void downButtonPressed();
};

#endif  // PBCEXPORTPDFDIALOG_H
