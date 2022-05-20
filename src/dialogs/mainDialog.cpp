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
#include "pbcController.h"
#include "models/pbcPlaybook.h"
#include "dialogs/pbcExportPdfDialog.h"
#include "dialogs/pbcDeleteDialog.h"
#include "dialogs/pbcNewPlaybookDialog.h"
#include "dialogs/pbcNewPlayDialog.h"
#include "dialogs/pbcOpenPlayDialog.h"
#include "dialogs/pbcSavePlayAsDialog.h"
#include "dialogs/pbcSetPasswordDialog.h"
#include "util/pbcDeclarations.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include "util/pbcStorage.h"
#include "util/pbcExceptions.h"
#include <QFileDialog>
#include <QStringList>
#include <QPushButton>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <string>
#include <vector>
#include <list>
#include <pbcVersion.h>
#include <iostream>

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
    ui(new Ui::MainDialog){
    ui->setupUi(this);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->actionNew_Play->setShortcut(QKeySequence("Ctrl+N"));
    ui->actionOpen_Play->setShortcut(QKeySequence("Ctrl+O"));
    ui->actionAdd_play_to_category->setShortcut(QKeySequence("Ctrl+E"));
    ui->actionSave_Play->setShortcut(QKeySequence("Ctrl+S"));
    ui->actionSave_Play_as->setShortcut(QKeySequence("Ctrl+Shift+S"));

    ui->actionNew_Playbook->setShortcut(QKeySequence("Ctrl+Alt+N"));
    ui->actionOpen_Playbook->setShortcut(QKeySequence("Ctrl+Alt+O"));
    ui->actionSave_Playbook_as->setShortcut(QKeySequence("Ctrl+Alt+S"));
    ui->actionImport_playbook->setShortcut(QKeySequence("Ctrl+Alt+I"));
    ui->actionPDF_Export->setShortcut(QKeySequence("Ctrl+Alt+W"));

    _currentPlay = _currentlySelectedPlays.begin();

    _playView = new PBCPlayView(NULL, this);
    ui->graphicsView->setScene(_playView);

    this->setMinimumWidth(PBCConfig::getInstance()->minWidth());
    this->setMinimumHeight(PBCConfig::getInstance()->minHeight());

    updateTitle(false);
}

void MainDialog::keyReleaseEvent(QKeyEvent *event) {
    if (event->modifiers() == Qt::ControlModifier) {
        switch (event->key()) {
            case Qt::Key_Right:
                nextPlay();
                break;
            case Qt::Key_Left:
                previousPlay();
                break;
        }
    }
}

/**
 * @brief shows the main window graphically at application startup
 */
void MainDialog::show(QString playbookPath) {
    QMainWindow::showMaximized();

    if (!playbookPath.isNull()) {
        loadPlaybook(playbookPath);
        return;
    }

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
        pbcAssert(false);
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
            PBCController::getInstance()->getPlaybook()->name();
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
    ui->generalInfoBox->setEnabled(true);
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
        pbcAssert(_playView != NULL);
        _playView->setSceneRect(0, 0, PBCConfig::getInstance()->canvasWidth(),
                                PBCConfig::getInstance()->canvasHeight());
        _playView->repaint();
    }
}

void MainDialog::wheelEvent(QWheelEvent *event) {
    if (event->delta() < 0) {
        nextPlay();
    } else {
        previousPlay();
    }
    event->accept();
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
    PBCNewPlayDialog dialog;
    int returnCode = dialog.exec();
    if (returnCode == QDialog::Accepted) {
        PBCNewPlayDialog::ReturnStruct rs = dialog.getReturnStruct();
        std::string name = rs.name;
        std::string codeName = rs.codeName;
        std::string formation = rs.formationName;

        _playView->createNewPlay(name, codeName, formation);

        enableMenuOptions();
    }
}


void MainDialog::fillPlayInfoDock(PBCPlaySP play) {
    ui->playNameLineEdit->setText(QString::fromStdString(play->name()));
    ui->codeNameLineEdit->setText(QString::fromStdString(play->codeName()));
    ui->commentTextEdit->setText(QString::fromStdString(play->comment()));
}

void MainDialog::fillPlayerInfoDock(PBCPlayerSP player) {
    if (player == NULL) {
        //reset the UI elements
        ui->playerNameLineEdit->setText("");
        ui->playerNrSpinBox->setValue(0);
        ui->routeBox->clear();
    } else {
        ui->playerNameLineEdit->setText(QString::fromStdString(player->name()));
        ui->playerNrSpinBox->setValue((int) player->nr());
        QColor color(player->color().r(), player->color().g(), player->color().b());
        ui->playerColorWheel->setColor(color);

        ui->routeBox->clear();
        int index = 0;
        int selected = 0;
        if (player->route() == NULL) {
            ui->routeBox->addItem("Select a route");
        }
        for (PBCRouteSP route : PBCController::getInstance()->getPlaybook()->routes()) {
            ui->routeBox->addItem(QString::fromStdString(route->name()));
            index++;
            if (player->route() != NULL && route->name() == player->route()->name()) {
                selected = index;
            }
        }
        ui->routeBox->setCurrentIndex(selected);
    }
}


/**
 * @brief Gets a play by name from the Playbook and loads
 * it into the embedded PBCPlayView
 */
void MainDialog::openPlay() {
    QMessageBox::StandardButton button =
            QMessageBox::warning(this,
                                 "Open Play",
                                 "This will discard unsaved changes to the current play. Do you want to continue?",  // NOLINT
                                 QMessageBox::Ok | QMessageBox::Cancel);
    if(button == QMessageBox::Ok) {
        PBCOpenPlayDialog dialog;
        int returnCode = dialog.exec();
        if (returnCode == QDialog::Accepted) {
            struct PBCOpenPlayDialog::ReturnStruct rs = dialog.getReturnStruct();  // NOLINT
            _currentlySelectedPlays = rs.filteredPlays;
            std::string playName = rs.playName;
            for(auto it = _currentlySelectedPlays.begin(); it != _currentlySelectedPlays.end(); it++) {
                if((*it)->name() == rs.playName) {
                    _currentPlay = it;
                }
            }
            _playView->setActivePlayer(NULL);
            _playView->showPlay((*_currentPlay)->name());
            updateTitle(true);
            enableMenuOptions();
        }
    }
}

void MainDialog::nextPlay() {
    if (_currentPlay != _currentlySelectedPlays.end()) {
        const auto& nextIt = std::next(_currentPlay);
        if (nextIt != _currentlySelectedPlays.end()) {
            _currentPlay++;
            _playView->showPlay((*_currentPlay)->name());
        }
    }
}

void MainDialog::previousPlay() {
    if (_currentPlay != _currentlySelectedPlays.begin()) {
        _currentPlay--;
        _playView->showPlay((*_currentPlay)->name());
    }
    /*
    const auto& playNames = PBCController::getInstance()->getPlaybook()->getPlayNames();
    if (_currentPlay != NULL) {
        const auto& it = std::find(playNames.begin(), playNames.end(), _currentPlay->name());
        if (it != playNames.end() && it != playNames.begin()) {
            const auto& prevIt = std::prev(it);
            pbcAssert(prevIt != playNames.end());
            const std::string prevName = *prevIt;
            showPlay(prevName);
        }
    }*/
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
 * and renames it according to "Name" and "Code Name" line edit fields
 */
void MainDialog::renameAndSavePlay() {
    std::string name = ui->playNameLineEdit->text().toStdString();
    std::string codename = ui->codeNameLineEdit->text().toStdString();
    try {
        _playView->renameAndSavePlay(name, codename);
    } catch(const PBCAutoSaveException& e) {
        QMessageBox::warning(this, "Save Play", "The play could not be saved. You have to save the playbook to a file before you can add plays.");  //NOLINT
        savePlaybookAs();
        return;
    }
    updateTitle(true);
}

/**
 * @brief Saves the current play displayed by the embedded PBCPlayView to the playbook
 */
void MainDialog::savePlay() {
    try {
        _playView->savePlay();
    } catch(const PBCAutoSaveException& e) {
        QMessageBox::warning(this, "Save Play", "The play could not be saved. You have to save the playbook to a file before you can add plays.");  //NOLINT
        savePlaybookAs();
        return;
    }
    updateTitle(true);
}

/**
 * @brief Saves the current play displayed by the embedded PBCPlayView with a
 * new name to the playbook.
 */
void MainDialog::savePlayAsWithDialog() {
    PBCSavePlayAsDialog dialog;
    int returnCode = dialog.exec();
    if (returnCode == QDialog::Accepted) {
        struct PBCSavePlayAsDialog::ReturnStruct rs = dialog.getReturnStruct();
        savePlayAs(rs.name, rs.codeName);
    }
}


/**
 * @brief Saves the current formation from the play displayed by the embedded
 * PBCPlayView with a new name to the playbook.
 */
void MainDialog::saveFormationAs() {
    bool ok;
    QString qformationname;
    while (true) {
        qformationname = QInputDialog::getText(
                this, "Save formation as", "formation name",
                QLineEdit::Normal, "", &ok);
        if (ok == true && qformationname == "") {
            QMessageBox::warning(this, "Save Formation as", "Cannot have empty formation name!");
        } else {
            break;
        }
    }

    if (ok == true) {
        pbcAssert(qformationname != "");
        std::string formationName = qformationname.toStdString();
        std::vector<std::string> formationNames = PBCController::getInstance()->getPlaybook()->getFormationNames();
        const auto &it = std::find(formationNames.begin(), formationNames.end(), formationName);
        if (it != formationNames.end()) {
            QMessageBox::StandardButton button =
                    QMessageBox::question(this,
                                          "Save Formation As",
                                          QString::fromStdString("There already exists a formation named '" + formationName +
                                          "'. Do you want to overwrite it?"),
                                          QMessageBox::Ok | QMessageBox::Cancel);
            if (button != QMessageBox::Ok) {
                QMessageBox::information(this, "Save Formation As", "Formation was not saved.");
                return;
            }
        }
        try {
            _playView->saveFormation(formationName);
        } catch (const PBCAutoSaveException &e) {
            QMessageBox::warning(this, "Save Formation As",
                    "Formation could not be saved. You have to save the playbook to a file before you can add formations");
            savePlaybookAs();

            return;
        }
    }
}

/**
 * @brief Resets the application and creates a new empty playbook
 */
void MainDialog::newPlaybook() {
    PBCNewPlaybookDialog dialog;
    int returnCode = dialog.exec();
    if (returnCode == QDialog::Accepted) {
        struct PBCNewPlaybookDialog::ReturnStruct rs = dialog.getReturnStruct();
        unsigned int playerNumber = rs.playerNumber;
        std::string name = rs.playbookTitle;

        pbcAssert(name != "");
        pbcAssert(playerNumber == 5 || playerNumber == 7 ||
            playerNumber == 9 || playerNumber == 11);
        PBCController::getInstance()->getPlaybook()->resetToNewEmptyPlaybook(name,
                                                            playerNumber);
        PBCStorage::getInstance()->init(name);
        resetForNewPlaybook();
        _playView->resetPlay();
        updateTitle(false);
    } else {
        pbcAssert(false);
    }
}

/**
 * @brief Saves the playbook persistently to a file after asking for a file name.
 */
void MainDialog::savePlaybookAs() {
    std::string stdFile = PBCController::getInstance()->getPlaybook()->name() + ".pbc";
    QFileDialog fileDialog(
                this, "Save Playbook",
                QString::fromStdString(stdFile),
                "PBC Files (*.pbc);;All Files (*.*)");

    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    if(fileDialog.exec() == true) {
        QStringList files = fileDialog.selectedFiles();
        pbcAssert(files.size() == 1);
        QString fileName = files.first();

        PBCSetPasswordDialog pwDialog;
        int returnCode = pwDialog.exec();
        if (returnCode == QDialog::Accepted) {
            QString password = pwDialog.getPassword();
            PBCStorage::getInstance()->savePlaybook(password.toStdString(),
                                                    fileName.toStdString());
            updateTitle(true);
            return;
        }
    }
    QMessageBox::warning(this, "Save Playbook", "Playbook was not saved!");
}

/**
 * @brief Loads a playbook from a given filename
 */
void MainDialog::loadPlaybook(QString fileName) {
    unsigned int decryptionFailureCount = 0;
    while (true) {
        bool ok;
        std::string msg;
        if (decryptionFailureCount == 0) {
            msg = "Enter decryption password";
        } else {
            msg = "Error on decryption. Maybe wrong password. Try again!";
        }
        QString password = QInputDialog::getText(this, "Open Playbook",
                                                 QString::fromStdString(msg),
                                                 QLineEdit::Password, "", &ok);
        if (ok == true) {
            try {
                PBCStorage::getInstance()->loadActivePlaybook(password.toStdString(),
                                                              fileName.toStdString());
            } catch (PBCDecryptionException &e) {
                if (decryptionFailureCount < PASSWORD_MAX_RETRYS - 1) {
                    decryptionFailureCount++;
                    continue;
                } else {
                    throw e;
                }
            } catch (PBCDeprecatedVersionException& e) {
                QMessageBox::critical(this,
                        "Open Playbook",
                        "Cannot load playbook because it's created by a newer version of Playbook-Creator. "
                        "Please download the latest version of Playbook-Creator!");
            }
            _playView->resetPlay();
            resetForNewPlaybook();
            updateTitle(true);
            break;
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
    if (fileDialog.exec() == true) {
        QStringList files = fileDialog.selectedFiles();
        pbcAssert(files.size() == 1);
        QString fileName = files.first();

        loadPlaybook(fileName);
        return;
    }
}


/**
 * @brief Import a playbook from a file which is specified by an open-dialog.
 */
void MainDialog::importPlaybook() {
    QFileDialog fileDialog(this, "Impport Playbook", "",
                           "PBC Files (*.pbc);;All Files (*.*)");

    fileDialog.setFileMode(QFileDialog::ExistingFile);
    if (fileDialog.exec() == true) {
        QStringList files = fileDialog.selectedFiles();
        pbcAssert(files.size() == 1);
        QString fileName = files.first();

        unsigned int decryptionFailureCount = 0;
        while (true) {
            bool ok;
            std::string msg;
            if (decryptionFailureCount == 0) {
                msg = "Enter decryption password";
            } else {
                msg = "Error on decryption. Maybe wrong password. Try again!";
            }
            QString password = QInputDialog::getText(this, "Import Playbook",
                                                     QString::fromStdString(msg),
                                                     QLineEdit::Password, "", &ok);
            if (ok == true) {
                QDialog* importDialog = new QDialog(this);
                importDialog->setWindowTitle("Choose what to import");
                QLayout* layout = new QVBoxLayout;
                QLineEdit* prefixLine = new QLineEdit;
                prefixLine->setPlaceholderText("prefix");
                QCheckBox* playCB = new QCheckBox("Import Plays");
                playCB->setChecked(true);
                QCheckBox* categoryCB = new QCheckBox("Import Categories");
                categoryCB->setChecked(true);
                QCheckBox* formationCB = new QCheckBox("Import Formations");
                formationCB->setChecked(true);
                QCheckBox* routeCB = new QCheckBox("Import Routes");
                routeCB->setChecked(false);
                layout->addWidget(playCB);
                layout->addWidget(categoryCB);
                layout->addWidget(formationCB);
                layout->addWidget(routeCB);
                layout->addWidget(prefixLine);
                QPushButton* okButton = new QPushButton("Import now!");
                layout->addWidget(okButton);
                importDialog->setLayout(layout);
                connect(okButton, &QPushButton::clicked, importDialog, &QDialog::accept);
                if (importDialog->exec() == QDialog::Accepted) {
                    bool import_plays = playCB->isChecked();
                    bool import_categories = categoryCB->isChecked();
                    bool import_formations = formationCB->isChecked();
                    bool import_routes = routeCB->isChecked();
                    std::string prefix = prefixLine->text().toStdString();
                    try {
                        PBCStorage::getInstance()->importPlaybook(
                                password.toStdString(),
                                fileName.toStdString(),
                                import_plays,
                                import_categories,
                                import_routes,
                                import_formations,
                                prefix);
                        QMessageBox::information(this,
                                                 "Import Playbook",
                                                 "Import successful. Your playbook has been saved automatically!");
                    }  catch (PBCImportException& e) {
                        QString msg = e.what();
                        msg.append("\n\nYou should rename or delete it and try to import again.");
                        QMessageBox::critical(this, "Import Playbook", msg);
                    } catch (PBCDecryptionException &e) {
                        if (decryptionFailureCount < PASSWORD_MAX_RETRYS - 1) {
                            decryptionFailureCount++;
                            continue;
                        } else {
                            throw e;
                        }
                    } catch (PBCDeprecatedVersionException& e) {
                        QMessageBox::critical(this,
                                              "Import Playbook",
                                              "Cannot load playbook because it's created by a newer version of Playbook-Creator. "
                                              "Please download the latest version of Playbook-Creator!");
                    }
                }
                break;
            }
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
        std::string stdFile = PBCController::getInstance()->getPlaybook()->name() + ".pdf";
        QFileDialog fileDialog(
                    this, "Export Playbook As PDF",
                    QString::fromStdString(stdFile),
                    "PDF Documents (*.pdf);;All Files (*.*)");

        fileDialog.setFileMode(QFileDialog::AnyFile);
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        if(fileDialog.exec() == true) {
            QStringList files = fileDialog.selectedFiles();
            pbcAssert(files.size() == 1);
            QString fileName = files.first();

            PBCStorage::getInstance()->exportAsPDF(fileName.toStdString(),
                                                   playListSP,
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
 * @brief Adds the current play to a category.
 *
 */
void MainDialog::addPlayToCategory() {
    try {
        _playView->editCategories();
    } catch(const PBCAutoSaveException& e) {
        QMessageBox::information(this, "", "You have to save the playbook before.");  //NOLINT
        savePlaybookAs();
        return;
    }
}

void MainDialog::deleteRoutes() {
    PBCDeleteDialog deleteDialog(DELETE_ENUM::DELETE_ROUTES, this);
    if (deleteDialog.exec() == QDialog::Accepted) {
        for (const auto &name : *deleteDialog.get_nameList()) {
            try {
                PBCController::getInstance()->getPlaybook()->deleteRoute(name.toStdString());
            } catch (PBCAutoSaveException &e) {
                // TODO log message
                /* The exception should not really be an issue here. If the playbook has not been saved to file yet,
                 * then there cannot be custom routes, plays, formations, etc. So the user can only delete default things.
                 * In the worst case he forgets saving the playbook and has to delete everything again. That's okay for now.
                 * It should be reworked as part of issue #20
                */
                continue;
            }
        }

    }
}

void MainDialog::deletePlays() {
    PBCDeleteDialog deleteDialog(DELETE_ENUM::DELETE_PLAYS, this);
    if (deleteDialog.exec() == QDialog::Accepted){
        for (const auto& name : *deleteDialog.get_nameList()) {
            try {
                PBCController::getInstance()->getPlaybook()->deletePlay(name.toStdString());
            } catch (PBCAutoSaveException &e) {
                // TODO log message (see issue #19)
                /* The exception should not really be an issue here. If the playbook has not been saved to file yet,
                 * then there cannot be custom routes, plays, formations, etc. So the user can only delete default things.
                 * In the worst case he forgets saving the playbook and has to delete everything again. That's okay for now.
                 * It should be reworked as part of issue #20
                */
                continue;
            }
        }
    }
}

void MainDialog::deleteFormations() {
    PBCDeleteDialog deleteDialog(DELETE_ENUM::DELETE_FORMATIONS, this);
    if (deleteDialog.exec() == QDialog::Accepted){
        for (const auto& name : *deleteDialog.get_nameList()) {
            try {
                PBCController::getInstance()->getPlaybook()->deleteFormation(name.toStdString());
            } catch (PBCAutoSaveException &e) {
                // TODO log message  (see issue #19)
                /* The exception should not really be an issue here. If the playbook has not been saved to file yet,
                 * then there cannot be custom routes, plays, formations, etc. So the user can only delete default things.
                 * In the worst case he forgets saving the playbook and has to delete everything again. That's okay for now.
                 * It should be reworked as part of issue #20
                */
                continue;
            }
        }
    }
}

void MainDialog::deleteCategories() {
    PBCDeleteDialog deleteDialog(DELETE_ENUM::DELETE_CATEGORIES, this);
    if (deleteDialog.exec() == QDialog::Accepted){
        for (const auto& name : *deleteDialog.get_nameList()) {
            try {
                PBCController::getInstance()->getPlaybook()->deleteCategory(name.toStdString());
            } catch (PBCAutoSaveException &e) {
                // TODO log message  (see issue #19)
                /* The exception should not really be an issue here. If the playbook has not been saved to file yet,
                 * then there cannot be custom routes, plays, formations, etc. So the user can only delete default things.
                 * In the worst case he forgets saving the playbook and has to delete everything again. That's okay for now.
                 * It should be reworked as part of issue #20
                */
                continue;
            }
        }
    }
}


/**
 * @brief The destructor
 */
MainDialog::~MainDialog() {
    delete ui;
}

void MainDialog::changeActivePlayerColor(QColor color) {
    if(color.isValid()) {
        this->_playView->setActivePlayerColor(PBCColor(color.red(),
                                                       color.green(),
                                                       color.blue()));
    }
}

void MainDialog::changeActivePlayerRoute(int index) {
    if (index > 0) {
        QString routeName = ui->routeBox->itemText(index);
        PBCRouteSP route = PBCController::getInstance()->getPlaybook()->getRoute(routeName.toStdString());
        this->_playView->setActivePlayerRoute(route);
    } else {
        // reset the route
        std::vector<PBCPathSP> emptyPaths;
        PBCRouteSP route(new PBCRoute("","", emptyPaths));
        this->_playView->setActivePlayerRoute(route);
    }

}

void MainDialog::changeActivePlayerName(QString name) {
    this->_playView->setActivePlayerName(name.toStdString());
}

void MainDialog::changeActivePlayerNr(int nr) {
    pbcAssert(nr >= 0 && nr <= UINT_MAX);
    this->_playView->setActivePlayerNr((unsigned int) nr);
}

void MainDialog::changePlayComment() {
    std::string comment = ui->commentTextEdit->toPlainText().toStdString();
    this->_playView->setPlayComment(comment);
}

void MainDialog::resetForNewPlaybook() {
    _currentlySelectedPlays = std::list<PBCPlaySP>();
    _currentPlay = _currentlySelectedPlays.begin();
}

void MainDialog::savePlayAs(std::string name, std::string codename) {
    std::vector<std::string> playNames = PBCController::getInstance()->getPlaybook()->getPlayNames();
    const auto& it = std::find(playNames.begin(), playNames.end(), name);
    if (it != playNames.end()) {
        QMessageBox::StandardButton button =
                QMessageBox::question(this,
                                      "Save Play As",
                                      QString::fromStdString("There already exists a play named '" + name + "'. Do you want to overwrite it?"),  // NOLINT
                                      QMessageBox::Ok | QMessageBox::Cancel);
        if(button != QMessageBox::Ok) {
            QMessageBox::information(this, "Save Play As", "Play was not saved.");
            return;
        }
    }
    try {
        _playView->savePlay(name, codename);
    } catch(const PBCAutoSaveException& e) {
        QMessageBox::warning(this, "Save Play As",
                             "The play could not be saved. You have to save the playbook to a file before you can add plays");
        savePlaybookAs();

        return;
    }
    updateTitle(true);
    _playView->showPlay(name);
}

void MainDialog::savePlayAsNamed() {
    std::string name = ui->playNameLineEdit->text().toStdString();
    std::string codename = ui->codeNameLineEdit->text().toStdString();
    savePlayAs(name, codename);
}

