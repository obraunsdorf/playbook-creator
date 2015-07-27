/** @file pbcPositionTranslator.h
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

#ifndef PBCPOSITIONTRANSLATOR_H
#define PBCPOSITIONTRANSLATOR_H

#include "pbcSingleton.h"
#include "pbcDeclarations.h"
#include "pbcConfig.h"

class PBCPositionTranslator : public PBCSingleton<PBCPositionTranslator> {
friend class PBCSingleton<PBCPositionTranslator>;
 private:
    // cppcheck-suppress unusedPrivateFunction
    static PBCDPoint ballPos();

 protected:
    PBCPositionTranslator();

 public:
    static PBCDPoint translatePos(PBCDPoint pos, PBCDPoint center = ballPos());
    static PBCDPoint retranslatePos(PBCDPoint pos,
                                    PBCDPoint center = ballPos());
};

#endif  // PBCPOSITIONTRANSLATOR_H
