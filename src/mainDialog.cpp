#include "mainDialog.h"
#include "ui_mainDialog.h"
#include "QResizeEvent"

#include "util/pbcConfig.h"
#include "models/pbcFormation.h"
#include "models/pbcPlay.h"
#include "gui/pbcPlayerView.h"

MainDialog::MainDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainDialog::show() {
    QMainWindow::show();

    unsigned int width = ui->graphicsView->width();
    unsigned int height = ui->graphicsView->height();
    PBCConfig::getInstance()->setCanvasSize(width - 2, height - 2);
    this->setMinimumWidth(PBCConfig::getInstance()->minWidth());
    this->setMinimumHeight(PBCConfig::getInstance()->minHeight());

    _gridIron = new PBCGridIronView(this);
    ui->graphicsView->setScene(_gridIron);
    _gridIron->setSceneRect(0, 0, PBCConfig::getInstance()->canvasWidth(),PBCConfig::getInstance()->canvasHeight());
}

void MainDialog::resizeEvent(QResizeEvent* e) {
    unsigned int width = ui->graphicsView->width();
    unsigned int height = ui->graphicsView->height();
    PBCConfig::getInstance()->setCanvasSize(width - 2, height - 2);
    if(e->oldSize().width() > 0) {
        _gridIron->setSceneRect(0, 0, PBCConfig::getInstance()->canvasWidth(),PBCConfig::getInstance()->canvasHeight());
        _gridIron->resize();
    }
}

void MainDialog::exit()
{
    QApplication::quit();
}

void MainDialog::showNewPlay() {
    _gridIron->setCurrentPlay();
}

MainDialog::~MainDialog()
{
    delete ui;
}
