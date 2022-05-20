/** @file pbcSettings.cpp
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

#include "pbcSettings.h"

#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <iostream>

const QString LAST_PLAYBOOK_LOCATION_KEY = "lastPlaybookLocation";
const QString DEFAULT_PLAYBOOK_LOCATION = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

void setLastPlaybookLocation(QFileInfo fileInfo) {
    QSettings settings;
    settings.setValue(LAST_PLAYBOOK_LOCATION_KEY, fileInfo.path());
}

QString getLastPlaybookLocation(QString fileName) {
    QSettings settings;

    QString lastPlaybookLocation =  settings.value(LAST_PLAYBOOK_LOCATION_KEY, DEFAULT_PLAYBOOK_LOCATION).toString();
    
    return  QDir(lastPlaybookLocation).absoluteFilePath(fileName);

}
