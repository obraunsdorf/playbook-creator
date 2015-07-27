/** @file pbcPlayView.cpp
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

#include "pbcPlayView.h"

#include "util/pbcConfig.h"
#include "gui/pbcPlayerView.h"
#include "QGraphicsEllipseItem"
#include "models/pbcPlaybook.h"
#include <string>

PBCPlayView::PBCPlayView(PBCPlaySP playSP, QObject *parent) :
    PBCGridIronView(parent),
    _currentPlay(playSP) {
    repaint();
}

void PBCPlayView::paintPlayName(unsigned int yPos,
                                unsigned int textSize,
                                PBCColor color) {
    QGraphicsTextItem* text = this->addText(QString::fromStdString(
                                                _currentPlay->codeName()),
                                            QFont("Helvetica",
                                                  textSize,
                                                  textSize,
                                                  true));
    text->setY(yPos);
    text->setDefaultTextColor(QColor(color.r(), color.g(), color.b()));
}


void PBCPlayView::repaint() {
    this->clear();
    paintLine(PBCConfig::getInstance()->losY(),
              PBCConfig::getInstance()->canvasWidth(),
              PBCConfig::getInstance()->losWidth(),
              PBCConfig::getInstance()->losColor());

    paintLine(PBCConfig::getInstance()->fiveYdY(),
              PBCConfig::getInstance()->canvasWidth(),
              PBCConfig::getInstance()->fiveYdWidth(),
              PBCConfig::getInstance()->fiveYdColor());

    paintBall(PBCConfig::getInstance()->canvasWidth() / 2,
              PBCConfig::getInstance()->losY());

    paintBorder();

    if(_currentPlay != NULL) {
        for(PBCPlayerSP playerSP : *(_currentPlay->formation())) {
            this->addItem(new PBCPlayerView(playerSP));
        }

        unsigned int textSize = PBCConfig::getInstance()->playNameSize();
        paintPlayName(PBCConfig::getInstance()->canvasHeight() - 2 * textSize,
                      textSize,
                      PBCConfig::getInstance()->playNameColor());
    }
}

void PBCPlayView::resetPlay() {
    _currentPlay = NULL;
    repaint();
}

void PBCPlayView::createNewPlay(const std::string &name,
                                const std::string &codeName,
                                const std::string &formationName) {
    PBCPlaySP play(new PBCPlay(name, codeName));
    if(formationName != "") {
        PBCFormationSP formation = PBCPlaybook::getInstance()->getFormation(formationName);  // NOLINT
        assert(formation != NULL);
        play->setFormation(formation);
    }
    _currentPlay = play;
    repaint();
}

void PBCPlayView::savePlay(const std::string &name,
                           const std::string &codeName) {
    if(name != "") {
        _currentPlay->setName(name);
        _currentPlay->setCodeName(codeName);
    }
    PBCPlaybook::getInstance()->addPlay(_currentPlay, true);
}


void PBCPlayView::showPlay(const std::string& name) {
    PBCPlaySP play = PBCPlaybook::getInstance()->getPlay(name);
    assert(play != NULL);
    _currentPlay = play;
    repaint();
}

void PBCPlayView::saveFormation(const std::string &formationName) {
    if(formationName != "") {
        _currentPlay->formation()->setName(formationName);
    }
    PBCPlaybook::getInstance()->addFormation(_currentPlay->formation());
}
