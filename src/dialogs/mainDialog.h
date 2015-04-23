#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QMainWindow>

#include "gui/pbcPlayView.h"
#include <string>

namespace Ui {
class MainDialog;
}

class MainDialog : public QMainWindow {
    Q_OBJECT

 private:
    Ui::MainDialog *ui;
    QString _currentPlaybookFileName;
    PBCPlayView* _playView;

    void updateTitle(bool saved);
    void enableMenuOptions();
    void resizeEvent(QResizeEvent* e);

 public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();
    void show();

 public slots:
    void exit();
    void showNewPlay();
    void openPlay();
    void savePlay();
    void savePlayAs();
    void saveFormationAs();
    void newPlaybook();
    void savePlaybookAs();
    void openPlaybook();
    void exportAsPDF();
    void showAboutDialog();
};

#endif  // MAINDIALOG_H
