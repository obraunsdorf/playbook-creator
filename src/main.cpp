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
#include "../updater/pbc_updater_bindings.h"
#include <botan/version.h>
#include <boost/version.hpp>
#include <QApplication>
#include <QMessageBox>
#include <iostream>

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
        UpdateCheckingStatus ucs = updates_available(PBC_VERSION_MAJOR, PBC_VERSION_MINOR, PBC_VERSION_PATCH); // TODO: numeric conversions might be problematic here!
        if (ucs.tag == UpdateCheckingStatus::Tag::UpdatesAvailable) {
            unsigned major = ucs.updates_available._0;
            uint64_t minor = ucs.updates_available._1;
            uint64_t patch = ucs.updates_available._2;
            std::string latest_version = "v" + std::to_string(major) +  "." + std::to_string(minor) + "." + std::to_string(patch);
            std::string msg = "A new version of PlaybookCreator is available (" + latest_version + "). "
                                "Please visit https://github.com/obraunsdorf/playbook-creator/releases";

            QMessageBox::information(&w, "PBC Update Checker", QString::fromStdString(msg),
                                     QMessageBox::Ok);
        } else if (ucs.tag == UpdateCheckingStatus::Tag::Error) {
            QMessageBox::information(&w, "PBC Update Checker", "An error occured during checking for updates"
                                                               " (maybe bad internet connection?). Please visit "
                                                               "https://github.com/obraunsdorf/playbook-creator/releases "
                                                               "and see if a new version of PBC has been released",
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
