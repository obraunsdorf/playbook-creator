#include "mainDialog.h"
#include "ui_mainDialog.h"
#include "QResizeEvent"

#include "util/pbcConfig.h"
#include "gui/pbcPlayerView.h"
#include "models/pbcPlaybook.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>

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

    _playView = new PBCPlayView(this);
    ui->graphicsView->setScene(_playView);
    _playView->setSceneRect(0, 0, PBCConfig::getInstance()->canvasWidth(),PBCConfig::getInstance()->canvasHeight());
}

void MainDialog::resizeEvent(QResizeEvent* e) {
    unsigned int width = ui->graphicsView->width();
    unsigned int height = ui->graphicsView->height();
    PBCConfig::getInstance()->setCanvasSize(width - 2, height - 2);
    if(e->oldSize().width() > 0) {
        _playView->setSceneRect(0, 0, PBCConfig::getInstance()->canvasWidth(),PBCConfig::getInstance()->canvasHeight());
        _playView->repaint();
    }
}

void MainDialog::exit()
{
    QApplication::quit();
}

void MainDialog::showNewPlay() {
    bool nameOk, codeNameOk, formationOk;
    QString name = QInputDialog::getText(this, "New Play", "name of the new Play", QLineEdit::Normal, "", &nameOk);
    QString codeName = QInputDialog::getText(this, "New Play", "code name of the new Play", QLineEdit::Normal, "", &codeNameOk);
    QString formation = QInputDialog::getItem(this, "New Play", "from which formation should the play be created?",
                                              _playView->getAvailableFormations(), 0, false, &formationOk);
    if((nameOk && codeNameOk && formationOk) == true) {

    }
    _playView->createNewPlay(name.toStdString(), codeName.toStdString(), formation.toStdString());

    // enable menu options
    QList<QAction*> actionList = this->findChildren<QAction*>();
    for(QAction* action : actionList) {
        if(action->isEnabled() == false) {
            action->setEnabled(true);
        }
    }
}

void MainDialog::openPlay()
{
    bool ok;
    QStringList playList = _playView->getAvailablePlays();
    if(playList.size() > 0) {
        QString play = QInputDialog::getItem(this, "Open Play", "choose a play", _playView->getAvailablePlays(), 0, false, &ok);
        if(ok == true) {
            assert(play != "");
            _playView->showPlay(play.toStdString());
        }
    } else {
        QMessageBox::information(this, "Open Play", "There is no play in your Playbook yet. Please create a new play first");
    }
}

void MainDialog::showAboutDialog()
{
    QMessageBox::about(this, QString("About"), QString("Playbook Creator by Oliver Braunsdorf"));
}

void MainDialog::savePlay()
{
    _playView->savePlay();
}

void MainDialog::savePlayAs()
{
    bool nameOk;
    bool codeOk;
    QString playName = QInputDialog::getText(this, QString("Save play as"), QString("play name"), QLineEdit::Normal, "", &nameOk);
    QString playCodeName = QInputDialog::getText(this, QString("Save play as"), QString("code name"), QLineEdit::Normal, "", &codeOk);
    if((nameOk && codeOk) == true) {
        assert(playName != "");
        _playView->savePlay(playName.toStdString(), playCodeName.toStdString());
    } else {
        // todo message to user
    }
}

void MainDialog::saveFormation()
{
    _playView->saveFormation();
}

void MainDialog::saveFormationAs()
{
    bool ok;
    QString formationName = QInputDialog::getText(this, QString("Save formation as"), QString("formation name"), QLineEdit::Normal, "", &ok);
    if(ok == true) {
        assert(formationName != "");
        _playView->saveFormation(formationName.toStdString());
    }
}

void MainDialog::savePlaybook()
{
    std::stringbuf buff;
    std::ostream ostream(&buff);
    boost::archive::text_oarchive archive(ostream);
    archive << *PBCPlaybook::getInstance();

    std::ofstream ofstream("/home/oliver/Desktop/myPlaybook.txt");
    ofstream << buff.str();
    ofstream.close();

    /*std::stringbuf buff;
    std::ostream ostream(&buff);
    PBCPlaybook::getInstance()->serialize(ostream);*/
    std::cout << buff.str() << std::endl;
}

void MainDialog::openPlaybook()
{
        std::ifstream ifstream("/home/oliver/Desktop/myPlaybook.txt");
        boost::archive::text_iarchive archive(ifstream);
        archive >> *PBCPlaybook::getInstance();
}

MainDialog::~MainDialog()
{
    delete ui;
}
