#include "mainDialog.h"
#include "ui_mainDialog.h"
#include "QResizeEvent"

#include "util/pbcConfig.h"
#include "gui/pbcPlayerView.h"
#include "models/pbcPlaybook.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include "util/pbcStorage.h"
#include <QFileDialog>
#include <string>

MainDialog::MainDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainDialog),
    _currentPlaybookFileName("") {
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

    _playView = new PBCPlayView(this);
    ui->graphicsView->setScene(_playView);
    _playView->setSceneRect(0, 0, PBCConfig::getInstance()->canvasWidth(),
                            PBCConfig::getInstance()->canvasHeight());
}

void MainDialog::enableMenuOptions() {
    QList<QAction*> actionList = this->findChildren<QAction*>();
    for(QAction* action : actionList) {
        if(action->isEnabled() == false) {
            action->setEnabled(true);
        }
    }
}

void MainDialog::resizeEvent(QResizeEvent* e) {
    unsigned int width = ui->graphicsView->width();
    unsigned int height = ui->graphicsView->height();
    PBCConfig::getInstance()->setCanvasSize(width - 2, height - 2);
    if(e->oldSize().width() > 0) {
        _playView->setSceneRect(0, 0, PBCConfig::getInstance()->canvasWidth(),
                                PBCConfig::getInstance()->canvasHeight());
        _playView->repaint();
    }
}

void MainDialog::exit() {
    QApplication::quit();
}

void MainDialog::showNewPlay() {
    bool nameOk, codeNameOk, formationOk;
    QString name = QInputDialog::getText(
                this, "New Play", "name of the new Play",
                QLineEdit::Normal, "", &nameOk);

    QString codeName = QInputDialog::getText(
                this, "New Play", "code name of the new Play",
                QLineEdit::Normal, "", &codeNameOk);

    QString formation = QInputDialog::getItem(
                this, "New Play",
                "from which formation should the play be created?",
                _playView->getAvailableFormations(), 0, false, &formationOk);

    if((nameOk && codeNameOk && formationOk) == true) {
        _playView->createNewPlay(
                    name.toStdString(),
                    codeName.toStdString(), formation.toStdString());

        enableMenuOptions();
    }
}

void MainDialog::openPlay() {
    bool ok;
    QStringList playList = _playView->getAvailablePlays();
    if(playList.size() > 0) {
        QString play = QInputDialog::getItem(
                    this, "Open Play", "choose a play",
                    _playView->getAvailablePlays(), 0, false, &ok);

        if(ok == true) {
            assert(play != "");
            _playView->showPlay(play.toStdString());
            enableMenuOptions();
        }
    } else {
        QMessageBox::information(
                    this, "Open Play",
                    "There is no play in your Playbook yet. Please create a new play first"); // NOLINT
    }
}

void MainDialog::showAboutDialog() {
    QMessageBox::about(this, "About", "Playbook Creator by Oliver Braunsdorf");
}

void MainDialog::savePlay() {
    _playView->savePlay();
}

void MainDialog::savePlayAs() {
    bool nameOk;
    bool codeOk;
    QString playName = QInputDialog::getText(
                this, "Save play as", "play name",
                QLineEdit::Normal, "", &nameOk);

    QString playCodeName = QInputDialog::getText(
                this, "Save play as", "code name",
                QLineEdit::Normal, "", &codeOk);

    if((nameOk && codeOk) == true) {
        assert(playName != "");
        _playView->savePlay(playName.toStdString(), playCodeName.toStdString());
    } else {
        // todo message to user
    }
}

void MainDialog::saveFormation() {
    QMessageBox::StandardButton button =
            QMessageBox::warning(this,
                                 "Save Formation",
                                 "This will overwrite the current formation. Are you sure?",  // NOLINT
                                 QMessageBox::Ok | QMessageBox::Cancel);

    if(button == QMessageBox::Ok) {
        _playView->saveFormation();
    }
}

void MainDialog::saveFormationAs() {
    bool ok;
    QString formationName = QInputDialog::getText(
                this, "Save formation as", "formation name",
                QLineEdit::Normal, "", &ok);

    if(ok == true) {
        assert(formationName != "");
        _playView->saveFormation(formationName.toStdString());
    }
}

void MainDialog::savePlaybook() {
    if(_currentPlaybookFileName == "") {
        savePlaybookAs();
    } else {
        PBCStorage::getInstance()->savePlaybook(
                    _currentPlaybookFileName.toStdString());
    }
}

void MainDialog::savePlaybookAs() {
    std::string stdFile = PBCPlaybook::getInstance()->name() + ".pbc";
    QFileDialog fileDialog(
                this, "Save Playbook",
                QString::fromStdString(stdFile),
                "PBC Files (*.pbc);;All Files (*.*)");

    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    if(fileDialog.exec() == true) {
        QStringList files = fileDialog.selectedFiles();
        assert(files.size() == 1);
        QString fileName = files.first();
        _currentPlaybookFileName = fileName;
        PBCStorage::getInstance()->savePlaybook(fileName.toStdString());
    }
}

void MainDialog::openPlaybook() {
    QFileDialog fileDialog(this, "Open Playbook", "",
                           "PBC Files (*.pbc);;All Files (*.*)");

    fileDialog.setFileMode(QFileDialog::ExistingFile);
    if(fileDialog.exec() == true) {
        QStringList files = fileDialog.selectedFiles();
        assert(files.size() == 1);
        QString fileName = files.first();
        _currentPlaybookFileName = fileName;
        PBCStorage::getInstance()->loadPlaybook(fileName.toStdString());
    }
}

MainDialog::~MainDialog() {
    delete ui;
}
