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
#include "util/pbcExceptions.h"
#include <QFileDialog>
#include <QPushButton>
#include <string>

MainDialog::MainDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainDialog),
    _currentPlaybookFileName("") {
    ui->setupUi(this);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    updateTitle(false);
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

    QMessageBox messageBox(this);
    QPushButton* openButton = messageBox.addButton("Open Playbook",
                                                  QMessageBox::AcceptRole);
    QPushButton* newButton = messageBox.addButton("New Playbook",
                                                   QMessageBox::AcceptRole);
    messageBox.exec();
    if(messageBox.clickedButton() == newButton) {
        newPlaybook();
    } else if(messageBox.clickedButton() == openButton) {  // NOLINT
        openPlaybook();
    } else {
        assert(false);
    }
}

void MainDialog::updateTitle(bool saved) {
    // TODO(obr): refactor this whole method and logic behind
    std::string windowTitle = "Playbook Creator V" +
            PBCConfig::getInstance()->version() +
            " - " +
            PBCPlaybook::getInstance()->name();
    if(saved == false) {
        windowTitle += "*";
    }

    this->setWindowTitle(QString::fromStdString(windowTitle));
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
    QMessageBox::StandardButton button =
            QMessageBox::warning(this,
                                 "Open Play",
                                 "This will discard unsaved changes to the current play. Do you want to continue?",  // NOLINT
                                 QMessageBox::Ok | QMessageBox::Cancel);

    if(button == QMessageBox::Ok) {
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
                    _playView->getAvailableFormations(), 0, false,
                    &formationOk);

        if((nameOk && codeNameOk && formationOk) == true) {
            _playView->createNewPlay(
                        name.toStdString(),
                        codeName.toStdString(), formation.toStdString());

            enableMenuOptions();
        }
    }
}

void MainDialog::openPlay() {
    bool ok;
    QStringList playList = _playView->getAvailablePlays();
    if(playList.size() > 0) {
        QMessageBox::StandardButton button =
                QMessageBox::warning(this,
                                     "Open Play",
                                     "This will discard unsaved changes to the current play. Do you want to continue?",  // NOLINT
                                     QMessageBox::Ok | QMessageBox::Cancel);

        if(button == QMessageBox::Ok) {
            QString play = QInputDialog::getItem(
                        this, "Open Play", "choose a play",
                        _playView->getAvailablePlays(), 0, false, &ok);

            if(ok == true) {
                assert(play != "");
                _playView->showPlay(play.toStdString());
                updateTitle(true);
                enableMenuOptions();
            }
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
    try {
        _playView->savePlay();
    } catch(const PBCStorageException& e) {
        QMessageBox::information(this, "", "You have to save the playbook to a file before you can add plays");  //NOLINT
        savePlaybookAs();
        return;
    }
    updateTitle(true);
}

void MainDialog::savePlayAs() {
    bool nameOk;
    bool codeOk;
    QString playName = QInputDialog::getText(this, "Save play as", "play name",
                                             QLineEdit::Normal, "", &nameOk);

    QString playCodeName = QInputDialog::getText(this, "Save play as",
                                                 "code name",
                                                 QLineEdit::Normal,
                                                 "", &codeOk);

    if((nameOk && codeOk) == true) {
        assert(playName != "");
        try {
            _playView->savePlay(playName.toStdString(),
                                playCodeName.toStdString());
        } catch(const PBCStorageException& e) {
            QMessageBox::information(this, "", "You have to save the playbook to a file before you can add plays");  //NOLINT
            savePlaybookAs();
            return;
        }
        updateTitle(true);
    } else {
        // TODO(obr): message to user
    }
}

void MainDialog::saveFormationAs() {
    bool ok;
    QString formationName = QInputDialog::getText(
                this, "Save formation as", "formation name",
                QLineEdit::Normal, "", &ok);

    if(ok == true) {
        assert(formationName != "");
        try {
            _playView->saveFormation(formationName.toStdString());
        } catch(const PBCStorageException& e) {
            QMessageBox::information(this, "", "You have to save the playbook to a file before you can add formations");  //NOLINT
            savePlaybookAs();
            return;
        }
    }
}

void MainDialog::newPlaybook() {
    bool ok;
    QString name = QInputDialog::getText(
                this, "New Playbook", "Playbook title",
                QLineEdit::Normal, "", &ok);

    if(ok == true) {
        assert(name != "");
        PBCPlaybook::getInstance()->resetToNewEmptyPlaybook(name.toStdString());
        PBCStorage::getInstance()->init(name.toStdString());
        updateTitle(false);
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

        bool ok;
        QString password = QInputDialog::getText(this, "Save Playbook",
                                                 "Enter encryption password",
                                                 QLineEdit::Password, "", &ok);
        if(ok == true) {
            _currentPlaybookFileName = fileName;
            PBCStorage::getInstance()->savePlaybook(password.toStdString(),
                                                    fileName.toStdString());
            updateTitle(true);
        }
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

        bool ok;
        QString password = QInputDialog::getText(this, "Open Playbook",
                                                 "Enter decryption password",
                                                 QLineEdit::Password, "", &ok);
        if(ok == true) {
            _currentPlaybookFileName = fileName;
            PBCStorage::getInstance()->loadPlaybook(password.toStdString(),
                                                    fileName.toStdString());
            _playView->resetPlay();
            updateTitle(true);
        }
    }
}

MainDialog::~MainDialog() {
    delete ui;
}
