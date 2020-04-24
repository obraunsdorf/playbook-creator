/** @file pbcOpenPlayDialog.h
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

#ifndef PBCOPENPLAYDIALOG_H
#define PBCOPENPLAYDIALOG_H

#include "models/pbcPlay.h"
#include <QDialog>
#include <QListWidgetItem>
#include <string>
#include <list>
#include <set>
#include <map>

namespace Ui {
class PBCOpenPlayDialog;
}

class PBCOpenPlayDialog : public QDialog {
    Q_OBJECT

 public:
    explicit PBCOpenPlayDialog(QWidget *parent = 0);
    ~PBCOpenPlayDialog();

    struct ReturnStruct {
        std::string playName;
        std::list<PBCPlaySP> filteredPlays;
    };

    struct ReturnStruct getReturnStruct();

 private:
    Ui::PBCOpenPlayDialog *ui;
    std::list<PBCPlaySP> _currentPlays;
    std::set<PBCCategorySP> _filteredCategories;

    void fillCategoryList(std::map<PBCCategorySP, unsigned int> categories,
                          unsigned int totalPlayCount);

 private slots:
    void filterCategory(QListWidgetItem* item);
    void reset();
};

#endif  // PBCOPENPLAYDIALOG_H
