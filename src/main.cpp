/** @file main.cpp
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
    @brief This is the main file.
*/
#include "dialogs/mainDialog.h"
#include "util/pbcExceptions.h"
#include "pbcVersion.h"
#include <botan/version.h>
#include <boost/version.hpp>
#include <QApplication>
#include <QMessageBox>
#include <iostream>

extern void* update_available(uint64_t major, uint64_t minor, uint64_t patch);
bool updateAvailable(int major, int minor, int patch) {
    auto result = update_available(major, minor, patch);

    return true;
}

/**
 * @brief the main function
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return Returns the return value of the Qt application execution.
 */
int main(int argc, char *argv[]) {
    std::cout << "Playbook Creator Version: "
              << PBCVersion::getVersionString()
              << std::endl;
    std::cout << "built with Qt version: " << QT_VERSION_STR << std::endl;
    std::cout << "built with boost version: " << BOOST_LIB_VERSION << std::endl;
    std::cout << "built with botan verison: "
              << BOTAN_VERSION_MAJOR << "."
              << BOTAN_VERSION_MINOR << "."
              << BOTAN_VERSION_PATCH << std::endl;

    QApplication a(argc, argv);
    MainDialog w;
    try {
        w.show();
        if (updateAvailable(PBC_VERSION_MAJOR, PBC_VERSION_MINOR, PBC_VERSION_PATCH)) {
            QMessageBox::information(&w, "PBC Update Checker", "A new version of PlaybookCreator is available. "
                                                              "Please visit https://github.com/obraunsdorf/playbook-creator/releases",
                                                              QMessageBox::Ok);
        }
        a.exec();
    } catch(std::exception &e) {
        QString errorString = "Terminating playbook creator. Please open an bug report on "
                              "Github (https://github.com/obraunsdorf/playbook-creator/issues) "
                              "containing the following error log.\n\n";
        errorString += e.what();
        QMessageBox::critical(QApplication::activeWindow(), "", errorString);
        return 1;
    }
    return 0;
}
