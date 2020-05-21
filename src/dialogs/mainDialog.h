/** @file mainDialog.h
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

#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QMainWindow>

#include "gui/pbcPlayView.h"
#include <string>
#include <QListWidgetItem>

namespace Ui {
class MainDialog;
}

class MainDialog : public QMainWindow {
    Q_OBJECT

 private:
    Ui::MainDialog *ui;
    PBCPlayView* _playView;
    std::list<PBCPlaySP> _currentlySelectedPlays;
    std::list<PBCPlaySP>::const_iterator _currentPlay;

    void resetForNewPlaybook();
    void updateTitle(bool saved);
    void enableMenuOptions();
    void resizeEvent(QResizeEvent* e);
    void wheelEvent(QWheelEvent *event);
    void savePlayAs(std::string name, std::string codename);


 public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();
    void show();
    void fillPlayerInfoDock(PBCPlayerSP player);
    void fillPlayInfoDock(PBCPlaySP play);
    void fillPlayScoutingInfoDock(PBCPlaySP play);
    void keyReleaseEvent(QKeyEvent *event);

 public slots:
    void exit();
    void showNewPlay();
    void openPlay();
    void nextPlay();
    void previousPlay();
    void savePlay();
    void savePlayAsWithDialog();
    void savePlayAsNamed();
    void saveFormationAs();
    void newPlaybook();
    void savePlaybookAs();
    void openPlaybook();
    void importPlaybook();
    void exportAsPDF();
    void showAboutDialog();
    void editCategories();
    void deleteRoutes();
    void deletePlays();
    void deleteFormations();
    void deleteCategories();
    void changePlayComment();
    void togglePlayEndzone(bool checked);
    void togglePlayQBOption(bool checked);
    void togglePlayQBRollout(bool checked);
    void toggleOtherCategory(QListWidgetItem* categoryItem);
    void changeActivePlayerColor(QColor color);
    void changeActivePlayerRoute(int index);
    void changeActivePlayerName(QString name);
    void changeActivePlayerNr(int nr);
};

#endif  // MAINDIALOG_H
