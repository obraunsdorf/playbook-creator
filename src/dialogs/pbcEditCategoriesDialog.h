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

#ifndef PBCEDITCATEGORIESDIALOG_H
#define PBCEDITCATEGORIESDIALOG_H

#include "models/pbcPlay.h"
#include <QDialog>
#include <string>

namespace Ui {
class PBCEditCategoriesDialog;
}

class PBCEditCategoriesDialog : public QDialog {
    Q_OBJECT

 public:
    explicit PBCEditCategoriesDialog(PBCPlaySP playSP, QWidget *parent = 0);  // NOLINT
    ~PBCEditCategoriesDialog();

    void editCategories();

 private:
    Ui::PBCEditCategoriesDialog *ui;
    PBCPlaySP _playSP;

    void refreshList();
    void updateCategoryAssignment();

 private slots:
    void createCategory();
};

#endif  // PBCEDITCATEGORIESDIALOG_H
