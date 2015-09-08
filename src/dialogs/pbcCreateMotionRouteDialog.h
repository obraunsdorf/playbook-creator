/** @file pbcCreateMotionRouteDialog.h
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

#ifndef PBCCREATEMOTIONROUTEDIALOG_H
#define PBCCREATEMOTIONROUTEDIALOG_H

#include <QDialog>
#include "models/pbcMotion.h"
#include "models/pbcRoute.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace Ui {
class PBCCreateMotionRouteDialog;
}

class PBCCreateMotionRouteDialog : public QDialog {
    Q_OBJECT

 private:
    Ui::PBCCreateMotionRouteDialog *ui;
    std::vector<PBCPathSP> _createdPaths;
    void execute();
    void saveRoute(PBCRouteSP routeSP);
    void savePlaybookOnRouteCreation();

 public:
    explicit PBCCreateMotionRouteDialog(QWidget *parent = 0);
    ~PBCCreateMotionRouteDialog();
    PBCMotionSP getCreatedMotion();
    PBCRouteSP getCreatedRoute();

 public slots:
    void addPath();
};

#endif   // PBCCREATEMOTIONROUTEDIALOG_H
