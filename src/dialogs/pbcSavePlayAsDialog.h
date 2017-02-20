/** @file pbcSavePlayAsDialog.h
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

#ifndef PBCSAVEPLAYASDIALOG_H
#define PBCSAVEPLAYASDIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
class PBCSavePlayAsDialog;
}

class PBCSavePlayAsDialog : public QDialog {
    Q_OBJECT

 public:
    explicit PBCSavePlayAsDialog(QWidget *parent = 0);
    ~PBCSavePlayAsDialog();

    struct ReturnStruct {
        std::string name;
        std::string codeName;
    };

    struct ReturnStruct getReturnStruct();
 private:
    Ui::PBCSavePlayAsDialog *ui;

    void accept();
};

#endif  // PBCSAVEPLAYASDIALOG_H
