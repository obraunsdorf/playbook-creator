#include "pbcPlayView.h"

#include "util/pbcConfig.h"
#include "gui/pbcPlayerView.h"
#include "QGraphicsEllipseItem"
#include "models/pbcPlaybook.h"

PBCPlayView::PBCPlayView(QObject *parent) :
    PBCGridIronView(parent)
{
    paintLine(PBCConfig::getInstance()->losY(), PBCConfig::getInstance()->canvasWidth(), PBCConfig::getInstance()->losWidth(), PBCConfig::getInstance()->losColor());
    paintLine(PBCConfig::getInstance()->fiveYdY(), PBCConfig::getInstance()->canvasWidth(), PBCConfig::getInstance()->fiveYdWidth(), PBCConfig::getInstance()->fiveYdColor());
    paintBall(PBCConfig::getInstance()->canvasWidth() / 2, PBCConfig::getInstance()->losY());
    paintBorder();
}


void PBCPlayView::repaint()
{
    this->clear();
    paintLine(PBCConfig::getInstance()->losY(), PBCConfig::getInstance()->canvasWidth(), PBCConfig::getInstance()->losWidth(), PBCConfig::getInstance()->losColor());
    paintLine(PBCConfig::getInstance()->fiveYdY(), PBCConfig::getInstance()->canvasWidth(), PBCConfig::getInstance()->fiveYdWidth(), PBCConfig::getInstance()->fiveYdColor());
    paintBall(PBCConfig::getInstance()->canvasWidth() / 2, PBCConfig::getInstance()->losY());
    paintBorder();
    if(_currentPlay != NULL) {
        for(PBCPlayerSP playerSP : *(_currentPlay->formation())) {
            this->addItem(new PBCPlayerView(playerSP));
        }
    }
}

void PBCPlayView::createNewPlay(const std::string &name, const std::string &codeName, const std::string &formationName)
{
    PBCPlaySP play(new PBCPlay(name, codeName));
    if(formationName != "") {
        PBCFormationSP formation = PBCPlaybook::getInstance()->getFormation(formationName);
        assert(formation != NULL);
        play->setFormation(formation);
    }
    _currentPlay = play;
    repaint();
}

void PBCPlayView::savePlay(const std::string &name, const std::string &codeName)
{
    if(name != "") {
        _currentPlay->setName(name);
        _currentPlay->setCodeName(codeName);
    }
    PBCPlaybook::getInstance()->addPlay(_currentPlay);
}


void PBCPlayView::showPlay(const std::string& name)
{
    PBCPlaySP play = PBCPlaybook::getInstance()->getPlay(name);
    assert(play != NULL);
    _currentPlay = play;
    repaint();
}

void PBCPlayView::saveFormation(const std::string &formationName)
{
    if(formationName != "") {
        _currentPlay->formation()->setName(formationName);
    }
    PBCPlaybook::getInstance()->addFormation(_currentPlay->formation());
}

QStringList PBCPlayView::getAvailableFormations() const
{
    QStringList result;
    for(PBCFormationSP formation : PBCPlaybook::getInstance()->formations()) {
        result.append(QString::fromStdString(formation->name()));
    }
    return result;
}

QStringList PBCPlayView::getAvailablePlays() const
{
    QStringList result;
    for(PBCPlaySP play : PBCPlaybook::getInstance()->plays()) {
        result.append(QString::fromStdString(play->name()));
    }
    return result;
}

