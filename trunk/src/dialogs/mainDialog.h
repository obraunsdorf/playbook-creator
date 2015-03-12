#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QMainWindow>

#include "gui/pbcPlayView.h"

namespace Ui {
class MainDialog;
}

class MainDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();
    void show();

private:
    Ui::MainDialog *ui;
    QString _currentPlaybookFileName;
    PBCPlayView* _playView;
    void resizeEvent(QResizeEvent* e);

public slots:
    void exit();
    void showNewPlay();
    void openPlay();
    void savePlay();
    void savePlayAs();
    void saveFormation();
    void saveFormationAs();
    void savePlaybook();
    void savePlaybookAs();
    void openPlaybook();
    void showAboutDialog();
};

#endif // MAINDIALOG_H
