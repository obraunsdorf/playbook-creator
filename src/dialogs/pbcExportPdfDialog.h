/** @file pbcExportPdfDialog.h
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
