/** @file pbcController.h
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

    Copyright 2015-2020 Oliver Braunsdorf

    @author Oliver Braunsdorf
    @brief PBCController shall be the central point for controlling the business logic of PBC.
*/

#ifndef PLAYBOOKCREATOR_PBCCONTROLLER_H
#define PLAYBOOKCREATOR_PBCCONTROLLER_H

#include "pbcController.h"
#include "models/pbcPlaybook.h"
#include "util/pbcSingleton.h"

class PBCPlaybook;
typedef boost::shared_ptr<PBCPlaybook> PBCPlaybookSP;

class PBCController : public PBCSingleton<PBCController> {
friend class PBCSingleton<PBCController>;
private:
    PBCPlaybookSP _playbook;
protected:
    PBCController();
public:
    PBCPlaybookSP getPlaybook();
};


#endif //PLAYBOOKCREATOR_PBCCONTROLLER_H
