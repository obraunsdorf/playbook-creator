/** @file mainDialog.cpp
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

#include "mainDialog.h"
#include "ui_mainDialog.h"
#include "QResizeEvent"

#include "util/pbcConfig.h"
#include "gui/pbcPlayerView.h"
#include "models/pbcPlaybook.h"
#include "dialogs/pbcExportPdfDialog.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include "util/pbcStorage.h"
#include "util/pbcExceptions.h"
#include <QFileDialog>
#include <QStringList>
#include <QPushButton>
#include <string>
#include <vector>
#include <list>
#include <pbcVersion.h>

/**
 * @class MainDialog
 * @brief This class creates the application's main dialog
 */

/**
 * @brief MainDialog::MainDialog The constructor
 * @param parent The parent widget this dialog belongs to. It should be NULL
 * since this is the main window which contains all other dialogs.
 */
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

/**
 * @brief shows the main window graphically at application startup
 */
void MainDialog::show() {
    QMainWindow::show();

    unsigned int width = ui->graphicsView->width();
    unsigned int height = ui->graphicsView->height();
    PBCConfig::getInstance()->setCanvasSize(width - 2, height - 2);
    this->setMinimumWidth(PBCConfig::getInstance()->minWidth());
    this->setMinimumHeight(PBCConfig::getInstance()->minHeight());

    _playView = new PBCPlayView(NULL, this);
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

/**
 * @brief adds / deletes an "*" to / from the window title depending on
 * saved / modified state of the current playbook
 * @param saved true if the playbook is saved, false if it is modified
 */
void MainDialog::updateTitle(bool saved) {
    // TODO(obr): refactor this whole method and logic behind
    std::string windowTitle = "Playbook Creator V" +
            PBCVersion::getSimpleVersionString() +
            " - " +
            PBCPlaybook::getInstance()->name();
    if(saved == false) {
        windowTitle += "*";
    }

    this->setWindowTitle(QString::fromStdString(windowTitle));
}

/**
 * @brief Enables all menu actions.
 *
 * Some of the menu actions are disabled at application startup, because they
 * don't make sense when no playbook is loaded (e.g. action for saving a playbook)
 */
void MainDialog::enableMenuOptions() {
    QList<QAction*> actionList = this->findChildren<QAction*>();
    for(QAction* action : actionList) {
        if(action->isEnabled() == false) {
            action->setEnabled(true);
        }
    }
}


/**
 * @brief A Qt dialog slot which is triggered when the main window is resized.
 *
 * It forces the embedded PBCPlayView to repaint adapting the new size
 * @param e contains resizing meta data
 */
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


/**
 * @brief Terminates the application
 */
void MainDialog::exit() {
    QApplication::quit();
}


/**
 * @brief Loads a new play into the embedded PBCPlayView
 */
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

        QStringList formationList;
        std::vector<std::string> formationNames =
                PBCPlaybook::getInstance()->getFormationNames();
        for(std::string& name : formationNames) {
            formationList.append(QString::fromStdString(name));
        }
        QString formation = QInputDialog::getItem(
                    this, "New Play",
                    "from which formation should the play be created?",
                    formationList, 0, false,
                    &formationOk);

        if((nameOk && codeNameOk && formationOk) == true) {
            _playView->createNewPlay(
                        name.toStdString(),
                        codeName.toStdString(), formation.toStdString());

            enableMenuOptions();
        }
    }
}


/**
 * @brief Gets a play by name from the Playbook and loads
 * it into the embedded PBCPlayView
 */
void MainDialog::openPlay() {
    bool ok;
    QStringList playList;
    std::vector<std::string> playNames =
            PBCPlaybook::getInstance()->getPlayNames();
    for(std::string& name : playNames) {
        playList.append(QString::fromStdString(name));
    }
    if(playList.size() > 0) {
        QMessageBox::StandardButton button =
                QMessageBox::warning(this,
                                     "Open Play",
                                     "This will discard unsaved changes to the current play. Do you want to continue?",  // NOLINT
                                     QMessageBox::Ok | QMessageBox::Cancel);

        if(button == QMessageBox::Ok) {
            QString play = QInputDialog::getItem(
                        this, "Open Play", "choose a play",
                        playList, 0, false, &ok);

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

/**
 * @brief Shows a simple dialog with information
 * about the application
 */
void MainDialog::showAboutDialog() {
    QMessageBox::about(this, "About", QString::fromStdString(PBCVersion::getVersionString()).prepend("Playbook Creator by Oliver Braunsdorf\nVersion: ")); //NOLINT
}

/**
 * @brief Saves the current play displayed by the embedded PBCPlayView to the playbook
 */
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

/**
 * @brief Saves the current play displayed by the embedded PBCPlayView with a
 * new name to the playbook.
 */
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


/**
 * @brief Saves the current formation from the play displayed by the embedded
 * PBCPlayView with a new name to the playbook.
 */
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

/**
 * @brief Resets the application and creates a new empty playbook
 */
void MainDialog::newPlaybook() {
    bool nameOk;
    bool playerNumberOk;
    QString name = QInputDialog::getText(
                this, "New Playbook", "Playbook title",
                QLineEdit::Normal, "", &nameOk);

    int playerNumber = QInputDialog::getInt(
                this, "New Playbook", "Number of offense players",
                5, 5, 11, 2, &playerNumberOk);

    if((nameOk && playerNumberOk) == true) {
        assert(name != "");
        assert(playerNumber == 5 || playerNumber == 7 ||
            playerNumber == 9 || playerNumber == 11);
        PBCPlaybook::getInstance()->resetToNewEmptyPlaybook(name.toStdString(),
                                                            playerNumber);
        PBCStorage::getInstance()->init(name.toStdString());
        updateTitle(false);
    }
}

/**
 * @brief Saves the playbook persistently to a file after asking for a file name.
 */
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

/**
 * @brief Loads a playbook from a file which is specified by an open-dialog.
 */
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

/**
 * @brief Exports the playbook to a PDF file.
 *
 * After file name and location have been asked, the PBCExportPDFDialog is
 * invoked to ask for the printing parameters. The PBCStorage is called, which
 * actually implements the PDF printing logic
 *
 */
void MainDialog::exportAsPDF() {
    PBCExportPDFDialog exportDialog;
    exportDialog.setWindowModality(Qt::ApplicationModal);
    boost::shared_ptr<PBCExportPDFDialog::ReturnStruct> returnStruct(new PBCExportPDFDialog::ReturnStruct());  //NOLINT
    boost::shared_ptr<QStringList> playListSP = exportDialog.exec(returnStruct);
    if(playListSP != NULL && playListSP->size() > 0) {
        std::string stdFile = PBCPlaybook::getInstance()->name() + ".pdf";
        QFileDialog fileDialog(
                    this, "Export Playbook As PDF",
                    QString::fromStdString(stdFile),
                    "PDF Documents (*.pdf);;All Files (*.*)");

        fileDialog.setFileMode(QFileDialog::AnyFile);
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        if(fileDialog.exec() == true) {
            QStringList files = fileDialog.selectedFiles();
            assert(files.size() == 1);
            QString fileName = files.first();

            std::list<boost::shared_ptr<PBCPlayView>> playViews;
            for(QString playName : *playListSP) {
                PBCPlaySP playSP = PBCPlaybook::getInstance()->getPlay(playName.toStdString()); //NOLINT
                boost::shared_ptr<PBCPlayView> playViewSP(new PBCPlayView(playSP));  //NOLINT
                playViews.push_back(playViewSP);
            }

            PBCStorage::getInstance()->exportAsPDF(fileName.toStdString(),
                                                   playViews,
                                                   returnStruct->paperWidth,
                                                   returnStruct->paperHeight,
                                                   returnStruct->columns,
                                                   returnStruct->rows,
                                                   returnStruct->marginLeft,
                                                   returnStruct->marginRight,
                                                   returnStruct->marginTop,
                                                   returnStruct->marginBottom);
        }
    }
}

/**
 * @brief The destructor
 */
MainDialog::~MainDialog() {
    delete ui;
}
