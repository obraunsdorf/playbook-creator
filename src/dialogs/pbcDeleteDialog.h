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

#ifndef PBCDELETEDIALOG_H
#define PBCDELETEDIALOG_H

#include <QDialog>
#include <boost/shared_ptr.hpp>

namespace Ui {
class PBCDeleteDialog;
}

enum DELETE_ENUM {
    DELETE_FORMATIONS,
    DELETE_ROUTES,
    DELETE_PLAYS,
    DELETE_CATEGORIES
};

class PBCDeleteDialog : public QDialog {
    Q_OBJECT

 public:
    explicit PBCDeleteDialog(DELETE_ENUM delete_enum, QWidget *parent = 0);
    ~PBCDeleteDialog();

    int exec();

 private:
    Ui::PBCDeleteDialog *ui;
    boost::shared_ptr<QStringList> _nameList;
public:
    const boost::shared_ptr<QStringList> get_nameList() const;
};

#endif  // PBCDELETEDIALOG_H
