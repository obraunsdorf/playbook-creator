#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QMainWindow>

#include "gui/pbcGridIronView.h"

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
    PBCGridIronView* _gridIron;
    void resizeEvent(QResizeEvent* e);
};

#endif // MAINDIALOG_H
